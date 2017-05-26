#include <ctime>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include "tEspacio.h"
#include <stdio.h>
#include <sys/resource.h>

//#include <graphics>

using namespace std;

void updateTimes(long &tMin, long &tMax,long &tiempoTotal, long tiempoFrame);
void resultados(long tMin, long tMax,long tiempoTotal, long tMed, int nParticulas);
void dibujar(tEspacio &espacio);




int main(int argc, char* argv[]) {

   if (argc==1){
      cout << endl << "ERROR: Indique el número de particulas" << endl;
      cout << "./gravity [numParticulas]" << endl;
   }
   else {

	   tEspacio espacio;    //Espacio que contiene lals particulas

	   int contador = 0;	// variables para medir los tiempos de calculo
	   long tMin = 100000;
	   long tMax = 0;
	   long tMed = 0;
	   long tiempoFrame = 0;
           long tiempoTotal = 0;

	   int n = atoi(argv[1]);

	   //Se crean n particulas random
	   int x = 0;	int y = 0;	int z = 0;	int masa = 0;
	   for (int i = 0; i < n; i++) {
		   x = rand() % 1600;
		   y = rand() % 900;
		   z = rand() % 1000;
		   masa = rand() % 100 + 50;
		   espacio.nuevaParticula(x, y, z, masa);
	   }

	   struct timeval start, end;
           long mtime, seconds, useconds;

	   cout << "Calculando frames..." << endl;
	   //Bucle infinito, cada tick calcula una unidad de tiempo
	   while (contador < 101) {
                   
		   
                   gettimeofday(&start, NULL);
		   
                   espacio.tick();     //Calculo  del siguiente frame
		   
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
   return 0;
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

