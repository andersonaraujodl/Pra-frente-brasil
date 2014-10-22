#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include<math.h>

//struct vetor
typedef struct {
    float y;
    float x;
    float angulo(){ //retorna angulo do vetor
        return atan(this->y/this/x);
    }

    float modulo (){
        return sqrt ((this->x*this->x + this->y * this->y));
    }

    //funcao velocidade
    void setValue(float modulo,float angulo){
        this->y = sin(radianos(angulo)) * modulo;
        this->x = cos(radianos(angulo)) * modulo;
    }

} vetor2d_type;

//struct corpo
typedef struct {
    vetor2d_type speed;
    vetor2d_type pos;
    float mass;
} corpo_type;



#endif // __PHYSICS_H__
