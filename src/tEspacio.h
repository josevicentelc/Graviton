#ifndef TESPACIO_H
#define TESPACIO_H

#include <vector>
#include "tParticula.h"

/**********************************************
Author: Jose Vicente Lozano Copa
Date:   15 / 06 / 2015

Representacion de un espacio que contiene particulas.
A cada "tick", el espacio calcula y aplica la fuerza gravitatoria mutua ejercida por cada una de las
particulas hacia el resto de particulas, lo que resulta en la orbita de estas en las tres dimensiones
**********************************************/

using namespace std;

class tEspacio
{
    public:

        tEspacio();

        //Crea una nueva particula en el espacio en las coordenadas x,y,z con una masa de m
        void nuevaParticula(float, float, float, float);

        //Crea una nueva particula en el espacio en las coordenadas x,y,z con una masa de m y le aplica una fuerza
        //inicial de fx,fy,fz
        void nuevaParticula(float x, float y, float z, float masa, float fx, float fy, float fz);

        //Retorna el listado de particulas
        vector<tParticula> getParticulas() {return espacio;};

        //Aplica las acciones debidas en una unidad de tiempo a todas las particulas del espacio
        void tick();

    private:
        vector<tParticula> espacio;
        void aplicarGravedad();
        void aplicarGravedad(tParticula &a, tParticula &b);
};

#endif // TESPACIO_H
