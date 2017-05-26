#include "tEspacio.h"
#include "tParticula.h"
#include <iostream>
#include <vector>
#include <math.h>

/**********************************************
Author: Jose Vicente Lozano Copa
Date:   15 / 06 / 2015
**********************************************/

using namespace std;

//*************************************************************************************************
//*************************************************************************************************
tEspacio::tEspacio(){
    //espacio = new vector<tParticula>; En c++ no es necesario construir los vectores
}
//*************************************************************************************************
//*************************************************************************************************
void tEspacio::nuevaParticula(float x, float y, float z, float masa, float fx, float fy, float fz)
 {
    tParticula a(x, y, z, masa);
    a.aplicarFuerza(fx, fy, fz);
    espacio.push_back(a);
 };
//*************************************************************************************************
//*************************************************************************************************
 void tEspacio::nuevaParticula(float x, float y, float z, float masa)
 {
     tParticula a(x, y, z, masa);
     espacio.push_back(a);
 };
//*************************************************************************************************
//*************************************************************************************************
void tEspacio::tick()
{
  //Este es el espacio del problema que es paralelizable
  aplicarGravedad();
  //****************************************************
  for (unsigned i = 0; i<espacio.size(); i++)
    if (!espacio[i].paraDestruir()) espacio[i].tick();
};
//*************************************************************************************************
//*************************************************************************************************
void tEspacio::aplicarGravedad()
{
    for (unsigned i = 0; i< espacio.size(); i++)
        for (unsigned j = i +1; j< espacio.size(); j++)
            if (!espacio[i].paraDestruir() && !espacio[j].paraDestruir())
                aplicarGravedad(espacio[i], espacio[j]);

    //Elimino del vector las que deben ser destruidas
     for (int i= espacio.size()-1; i>=0; i--)
        if (espacio[i].paraDestruir())   espacio.erase(espacio.begin() + i);
};
//*************************************************************************************************
//*************************************************************************************************
void tEspacio::aplicarGravedad(tParticula &a, tParticula &b)
{
    //Constante de gravitacion universal de Newton
    float G = 0.0000667384f;

    //Obtengo la distancia entre las particulas a y b con el teorema de Pitagoras
    float distancia = (float) sqrt( pow(a.x()-b.x(), 2) + pow(a.y()-b.y(), 2)+ pow(a.z()-b.z(), 2) );

    /**
        Antes de calcular el efecto de la gravedad, voy a detectar si las dos particulas colisionan
        tomando sus radios y la distancia que las separa
    */

    //Si la distancia que las separa es menor que la suma de sus radios, estonces se estan tocando
    if (distancia <= a.getRadio() + b.getRadio())
    {
        a.colision(b);
    }
    else
    {
        float maximo =  50000;   //limites de la pantalla
        float minimo = -50000;

        if (a.x() > maximo || a.x() < minimo || a.y() > maximo || a.y()< minimo || a.z() > maximo || a.z() < minimo)  a.Destruir();
        if (b.x() > maximo || b.x() < minimo || b.y() > maximo || b.y()< minimo || b.z() > maximo || b.z() < minimo)  b.Destruir();

        //Calculo la fuerza gravitatoria con la Ley de gravedad de Newton
        float fuerza = (float) (G * (a.getMasa() * b.getMasa()) / pow(distancia, 2));

        //Descompongo la fuerza en los tres ejes cartesianos x,y,z
        float fuerzaX = fuerza * ((a.x()-b.x()) / distancia);
        float fuerzaY = fuerza * ((a.y()-b.y()) / distancia);
        float fuerzaZ = fuerza * ((a.z()-b.z()) / distancia);

        //y aplico la fuerza a ambas particulas
        b.aplicarFuerza(fuerzaX, fuerzaY, fuerzaZ);
        //pero en sentidos opuestos
        a.aplicarFuerza(fuerzaX*-1, fuerzaY*-1, fuerzaZ*-1);
    }
}







