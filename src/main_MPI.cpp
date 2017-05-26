#include <ctime>
#include <mpi.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include "tEspacio_MPI.h"
#include "tParticula.h"
#include <stdio.h>
#include <sys/resource.h>

//#include <graphics>

using namespace std;

void enviarVector(vector<tParticula> &particulas, int idProceso, int nProcs);
void recibirVector(vector<tParticula> &particulas, int idProceso, int nProcs);
void updateTimes(long &tMin, long &tMax,long &tiempoTotal, long tiempoFrame);
void resultados(long tMin, long tMax,long tiempoTotal, long tMed, int nParticulas);
void dibujar(tEspacio &espacio);




int main(int argc, char* argv[]) {

   if (argc!=2){
      cout << endl << "ERROR: Indique el número de particulas" << endl;
      cout << "./gravity [numParticulas]" << endl;
   }
   else {

           int idProceso, nProcs;
           MPI_Init(&argc, &argv);
           MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
           MPI_Comm_rank(MPI_COMM_WORLD, &idProceso);
	   //mpi::comunicator world;
           //cout << "Hola, soy el proceso " << idProceso << " de [0,"<< (nProcs-1)<<"]" << endl;

	   tEspacio espacio(idProceso, nProcs);    //Espacio que contiene lals particulas

	   int contador = 0;	// variables para medir los tiempos de calculo
	   long tMin = 100000;
	   long tMax = 0;
	   long tMed = 0;
	   long tiempoFrame = 0;
           long tiempoTotal = 0;

	   int n = atoi(argv[1]);


	  // if (idProceso == 0){	//Solo el primer proceso crea el vector, que despues se debe transferir al resto de equipos
	  	   cout << "Generando vector de " << n << " particulas";
         	   //Se crean n particulas random
		   int x = 0;	int y = 0;	int z = 0;	int masa = 0;
		   for (int i = 0; i < n; i++) {
			   x = rand() % 1600;
			   y = rand() % 900;
			   z = rand() % 1000;
			   masa = rand() % 100 + 50;
			   espacio.nuevaParticula(x, y, z, masa);
		   }
	     	   cout << " [>]"<<espacio.getParticulas().size()<<"[<]"<<endl;
	  // }

	  

	   struct timeval start, end;
           long mtime, seconds, useconds;
	   vector<tParticula> particulas;


	   //Bucle infinito, cada tick calcula una unidad de tiempo
	   while (contador < 101) {

                   if (idProceso == 0) gettimeofday(&start, NULL);

		   particulas = espacio.getParticulas();
		   //enviarVector(particulas, idProceso, nProcs);	                //Envio las particulas a los trabajadores
                   espacio.tick();     						//Calculo  del siguiente frame
		   //recibirVector(particulas, idProceso, nProcs);	                //Recibo las particulas de los trabajadores y me quedo con las que cada uno calcula
		   
		   if (idProceso == 0) {					//Solo el maestro hace calculos de tiempos
		           gettimeofday(&end, NULL);
		           seconds = (end.tv_sec - start.tv_sec);
		           useconds = (end.tv_usec - start.tv_usec);
		           mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
		           //dibujar(espacio);   //Se dibujan las particulas, no se implementa en C++
			   tiempoFrame = mtime;
			   tMed += tiempoFrame;
			   updateTimes(tMin, tMax, tiempoTotal, tiempoFrame);
			   contador++;
			   cout << contador << "...("<<tiempoFrame<<" ms)"<<endl;
			   if (contador == 100) {
		                   cout << endl;
				   tMed /= contador;
				   resultados(tMin, tMax, tMed, tiempoTotal, n);
			   }
		   }
	   }
        MPI_Finalize();
   }
   return 0;
}
//**********************************************************************************************************************
void enviarVector(vector<tParticula> & particulas, int idProceso, int nProcs){
	
	//Numero de bytes del vector de particulas
	float datosParticula[8]; //posvector, x, y, z, masa, fx, fy, fz
	
	//Si soy el proceso 0, transfiero el vector a los trabajadores
	if (idProceso == 0){
		cout << "Maestro, envio del vector a los workers";
		//cout << "Soy maestro, envio las particulas a "<<endl; 
		for (int i=1;i< nProcs; i++){	//Para casa uno de los trabajadores
			cout << " worker -> " << i << endl;
			for (int j=0; j< particulas.size(); j++){//Le mando cada una de las particulas
				datosParticula[0] = j;
				datosParticula[0] = particulas[j].x();
				datosParticula[0] = particulas[j].y();
				datosParticula[0] = particulas[j].z();
				datosParticula[0] = particulas[j].getMasa();
				datosParticula[0] = particulas[j].velocidadX();
				datosParticula[0] = particulas[j].velocidadY();
				datosParticula[0] = particulas[j].velocidadZ();
				MPI_Send(&datosParticula, 8, MPI_FLOAT, i, 0, MPI_COMM_WORLD);	
			}
		}	
		cout << " [ok]"<<endl;
	}else{
		//Si soy un trabajador, recibo el vector del maestro
		//Se esta recibiendo un flujo de floats por MPI de 8 en 8, con cada 8 compongo una particula
		cout << "Worker " << idProceso << " recibiendo vector de maestro ";
		for (int i = 0; i< particulas.size(); i++){	//Espero una recepcion por cada particula del vector
			//Recibo un conjunto de 8 floats 
			MPI_Recv(&datosParticula, 8, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//con los 8 floats, recompongo la particula
			particulas[datosParticula[0]] = tParticula(
								datosParticula[1],	//pos X
								datosParticula[2],	//pos Y
								datosParticula[3],	//pos Z
								datosParticula[4],	//Masa
								datosParticula[5],	//Vel X
								datosParticula[6],	//vel Y
								datosParticula[7]);	//vel Z
		}
		cout << " [ok]"<<endl;
	
	}

}
//**********************************************************************************************************************
void recibirVector(vector<tParticula> & particulas, int idProceso, int nProcs){
	
	int parPorTrabaj = particulas.size() / nProcs; 	//Numero de particulas asignadas a cada trabajador
	float datosParticula[8]; 			//posvector, x, y, z, masa, fx, fy, fz

	//Si soy un trabajador, envio al maestro las particulas que he calculado
	if (idProceso!=0){
		cout << "Worker, envio vector calculado al maestro ";
		for (int i=parPorTrabaj*idProceso; i<parPorTrabaj*(idProceso+1) && i<particulas.size(); i++){
				datosParticula[0] = i;
				datosParticula[0] = particulas[i].x();
				datosParticula[0] = particulas[i].y();
				datosParticula[0] = particulas[i].z();
				datosParticula[0] = particulas[i].getMasa();
				datosParticula[0] = particulas[i].velocidadX();
				datosParticula[0] = particulas[i].velocidadY();
				datosParticula[0] = particulas[i].velocidadZ();
				MPI_Send(&datosParticula, 8, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);	
		}
		cout << " [ok]"<<endl;
	//Si soy el maestro, recibo las particulas asignadas a cada trabajador 
	}else{
		for (int i=1; i<nProcs; i++){	//Por cada uno de los trabajadores
			cout << "Maestro, recibo vector del worker "<<i<<" ";
			for (int j=parPorTrabaj*i; i<parPorTrabaj*(i+1) && i<particulas.size(); j++){ //Espero cada una de las particulas asignadas
			//Recibo un conjunto de 8 floats 
			MPI_Recv(&datosParticula, 8, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//con los 8 floats, recompongo la particula
			particulas[datosParticula[0]] = tParticula(
								datosParticula[1],	//pos X
								datosParticula[2],	//pos Y
								datosParticula[3],	//pos Z
								datosParticula[4],	//Masa
								datosParticula[5],	//Vel X
								datosParticula[6],	//vel Y
								datosParticula[7]);	//vel Z
			}
			cout << " [ok]"<<endl;
		}
	}

}
//**********************************************************************************************************************




void dibujar(tEspacio &espacio) {
    //No implementado en C++

        /*//METODOS PARA DIBUJAR LAS ESFERAS EN PANTALLA, no implementado en la version c++
        for (int i=0;i<espacio.getParticulas().size();i++){
		//Si supiese como, aqui dibujaria los circulo en un entorno grafico
		//Coordenada x -------> espacio.getParticulas()[i].x();
		//Coordenada y -------> espacio.getParticulas()[i].y();
		//Radio del circulo --> espacio.getParticulas()[i].getRadio();
        }*/

}

void resultados(long tMin, long tMax, long tMed, long tiempoTotal, int nParticulas) { // muestra por pantalla una tabla con los resultados temporales
	cout << "------Tabla de Tiempos para " << nParticulas << " Partículas-------" << endl;
        cout << "Tiempo máximo : "<< tMax << " ms"<<endl;
        cout << "Tiempo mínimo : "<< tMin << " ms"<<endl;
        cout << "Tiempo medio  : "<< tMed << " ms"<<endl;
        cout << "Tiempo total  : "<< tiempoTotal << " ms"<<endl;
	cout << "--------------------------------------------------" << endl;
}

void updateTimes(long &tMin, long &tMax,long &tiempoTotal, long tiempoFrame) { // metodo que actualiza los tiempos maximos y minimos para el calculo de un frame
	if (tiempoFrame < tMin)
		tMin = tiempoFrame;
	if (tiempoFrame > tMax)
		tMax = tiempoFrame;
    tiempoTotal += tiempoFrame;
}

