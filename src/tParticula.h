#ifndef TPARTICULA_H
#define TPARTICULA_H

/**********************************************
Author: Jose Vicente Lozano Copa
Date:   15 / 06 / 2015
**********************************************/


class tParticula
{
    public:
        tParticula();
        tParticula(float, float, float, float);
	tParticula(float, float, float, float, float, float, float);
        virtual ~tParticula();

        float x() {return posicionX;};
        float y() {return posicionY;};
        float z() {return posicionZ;};

        float velocidadX() {return velX;};
        float velocidadY() {return velY;};
        float velocidadZ() {return velZ;};

        void setVelocidadX(float newvx) {velX = newvx;};
        void setVelocidadY(float newvy) {velY = newvy;};
        void setVelocidadZ(float newvz) {velZ = newvz;};

        void aplicarFuerza(float, float, float);
        void tick();

        float getFuerzaX();
        float getFuerzaY();
        float getFuerzaZ();

        float getMasa(){return masa;};
        void setMasa(float nuevamasa);

        float getDensidad() {return densidad;};
        void setDensidad(float nuevaDensidad);
        int getRadio() {return radio;};
        void Destruir() {destruir = true;};
        bool paraDestruir() {return destruir;};

        //Retorna true si la particula this esta en colision con la particula por referencia y
        //recalcula las velocidades de ambos cuerpos tratandolas como una colision perfectamente inelastica
        void colision(tParticula &otra);

    protected:
    private:

        void calcularRadio();
        float posicionX;
        float posicionY;
        float posicionZ;
        float velX;
        float velY;
        float velZ;
        float masa;
        float densidad;
        int radio;
        float elasticidad;
        bool destruir;
        int multiplicadorMasa;


};

#endif // TPARTICULA_H
