#include "tParticula.h"
#include <iostream>
#include <stdio.h>
#include <math.h>

/**********************************************
Author: Jose Vicente Lozano Copa
**********************************************/

using namespace std;

//****************************************************************************************
//****************************************************************************************
tParticula::tParticula()
{
        multiplicadorMasa = 100;
        posicionX   = 0;
        posicionY   = 0;
        posicionZ   = 0;
        velX        = 0;
        velY        = 0;
        velZ        = 0;
        masa        = 1;
        densidad    = 1;
        radio       = 1;
        calcularRadio();
        destruir = false;
};

//****************************************************************************************
//****************************************************************************************
// nx       => posicion en el eje x
// ny       => posicion en el eje y
// ny       => posicion en el eje y
// nmasa    => masa de la particula en kg*10^6
tParticula::tParticula(float nx, float ny, float nz, float nmasa)
{
        multiplicadorMasa = 100;
        posicionX   = nx;
        posicionY   = ny;
        posicionZ   = nz;
        velX        = 0;
        velY        = 0;
        velZ        = 0;
        masa        = nmasa*multiplicadorMasa;
        densidad    = 1;
        radio       = 1;
        calcularRadio();
        destruir = false;
};

//****************************************************************************************
//****************************************************************************************
// nx       => posicion en el eje x
// ny       => posicion en el eje y
// ny       => posicion en el eje y
// nmasa    => masa de la particula en kg*10^6
tParticula::tParticula(float nx, float ny, float nz, float nmasa, float vx, float vy, float vz)
{
        multiplicadorMasa = 100;
        posicionX   = nx;
        posicionY   = ny;
        posicionZ   = nz;
        velX        = vx;
        velY        = vy;
        velZ        = vz;
        masa        = nmasa*multiplicadorMasa;
        densidad    = 1;
        radio       = 1;
        calcularRadio();
        destruir = false;
};//****************************************************************************************
//****************************************************************************************

/*Retorna true si la particula this esta en colision con la particula por referencia y
recalcula las velocidades de ambos cuerpos tratandolas como una colision perfectamente inelastica*/

void tParticula::colision(tParticula &otra)
{
    float distancia = (float) sqrt( pow(x()-otra.x(), 2) + pow(y()-otra.y(), 2)+ pow(z()-otra.z(), 2) );

    if (distancia <= getRadio() + otra.getRadio())
    {
        float vthisx = velX;
        float vthisy = velY;
        float vthisz = velZ;

        float votrax = otra.velocidadX();
        float votray = otra.velocidadY();
        float votraz = otra.velocidadZ();

        float m1 = getMasa();
        float m2 = otra.getMasa();
        float e = 0.8f;    //Coeficiente de restitucion


        velX    =  (((m1 - m2*e) * vthisx) + (m2 * (1+e) * votrax)) / (m1 + m2);
        velY    =  (((m1 - m2*e) * vthisy) + (m2 * (1+e) * votray)) / (m1 + m2);
        velZ    =  (((m1 - m2*e) * vthisz) + (m2 * (1+e) * votraz)) / (m1 + m2);

        float otravelx = (((m2 - m1*e) * votrax) + (m1 * (1+e) * vthisx)) / (m1 + m2);;
        float otravely = (((m2 - m1*e) * votray) + (m1 * (1+e) * vthisy)) / (m1 + m2);;
        float otravelz = (((m2 - m1*e) * votraz) + (m1 * (1+e) * vthisz)) / (m1 + m2);;

        otra.setVelocidadX(otravelx);
        otra.setVelocidadY(otravely);
        otra.setVelocidadZ(otravelz);
    }
};


/*Establece la nueva masa de la particula y recalcula el radio*/

void tParticula::setMasa(float nuevamasa)
{
    masa = nuevamasa;
    masa = masa * multiplicadorMasa;
    calcularRadio();
};

//****************************************************************************************
//****************************************************************************************
/* Establece la nueva densidad de la particula y recalcula el radio*/
void tParticula::setDensidad(float nuevaDensidad)
{
    if (nuevaDensidad>0)
    {
        densidad = nuevaDensidad;
        calcularRadio();
    }
};


//****************************************************************************************
//****************************************************************************************
/*
    Calcula el radio de una esfera dada su masa y su densidad

    El volumen de una esfera es 4/3pi*r^3

    La constante 4.188790205 = (4/3)pi

    Elevar al exponente 0.333333 es equivalente a obtener la raiz cúbica

*/
void tParticula::calcularRadio()
{
    float volumen = masa / multiplicadorMasa / densidad;
    radio = pow (volumen / (4.188790205), 0.333333);
};


//****************************************************************************************
//****************************************************************************************
/*
    Mueve la particula dada su velocidad en los tres ejes la cantidad de una una unidad de tiempo
*/
void tParticula::tick()
{
    posicionX = posicionX + velX;
    posicionY = posicionY + velY;
    posicionZ = posicionZ + velZ;
}

//****************************************************************************************
//****************************************************************************************
//  Fuerza de un objeto por el hecho de tener una velocidad y una masa
//  La F = m*v
float tParticula::getFuerzaX(){ return  masa*velX;};
float tParticula::getFuerzaY(){ return  masa*velY;};
float tParticula::getFuerzaZ(){ return  masa*velZ;};


//****************************************************************************************
//****************************************************************************************
/*
    El metodo de aplicarFuerza, calcula la nueva velocidad en X,Y,Z
    de la particula a partir de la aceleracion producida por la fuerza aplicada
*/
void tParticula::aplicarFuerza(float vectorx, float vectory, float vectorz)
{
    float aceleracionx = vectorx / masa;
    float aceleraciony = vectory / masa;
    float aceleracionz = vectorz / masa;
    setVelocidadX(velocidadX() + aceleracionx);
    setVelocidadY(velocidadY() + aceleraciony);
    setVelocidadZ(velocidadZ() + aceleracionz);
};


//****************************************************************************************
//****************************************************************************************
tParticula::~tParticula()
{}
