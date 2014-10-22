#include <iostream>
#include "physics.h"


/**
 * @brief Funç~ao de convers~ao de graus em radianos
 * @param Valor do angulo em graus
 * @return valor do angulo em radianos
 */
float radianos(float ang){
    return (ang/180)*M_PI;
}


/**
 * @brief Calcula a nova posiç~ao do objeto em trajet´oria obliqua (sem alteraç~ao em x)
 * @param p Corpo sobre a qual a nova posiç~ao ser´a calculada
 * @param dt delta tempo
 */
void lancamento(corpo_type *p, float dt){
    p->pos.x = p->pos.x +(p->speed.x*dt);
    p->pos.y = p->pos.y+(p->speed.y*dt)+(g*(dt*dt))/2;
}

/**
 * @brief Calcula o efeito da colis~ao sobre os corpos
 * @param c Corpo 1
 * @param p Corpo 2
 */
void colisao (corpo_type *c, corpo_type *p){
    vetor2d_type c_v_ini;

    c_v_ini = c->speed;

    c->speed = sqrt(p->mass * p->speed * p->speed + p->mass * p->speed * p->speed / c->mass);
    p->speed = sqrt(c->mass * c_v_ini.speed * c_v_ini.speed + c->mass * c->speed * c->speed / p->mass);
}


//função atrito
void atrito (corpo_type *a, float coefatrito, float dt){
    a->speed.x = (coefatrito * g) * (dt);
}
