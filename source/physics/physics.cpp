#include <iostream>
#include "physics.h"


/**
 * @brief Função de conversão de graus em radianos
 * @param Valor do angulo em graus
 * @return valor do angulo em radianos
 */
float radianos(float ang){
    return (ang/180)*M_PI;
}


/**
 * @brief Calcula a nova posição do objeto em trajetória obliqua (sem alteração em x)
 * @param p Corpo sobre a qual a nova posição será calculada
 * @param dt delta tempo
 */
void lancamento(game_object_type *p, float dt){
    p->body.pos.x = p->body.pos.x +(p->body.speed.x*dt);
    p->body.speed.y = p->body.speed.y +GRAVIDADE;
	p->body.pos.y = p->body.pos.y + (p->body.speed.y*dt) +(GRAVIDADE*(dt*dt))/2 ; 
}

/**
 * @brief Calcula o efeito da colisão sobre os corpos
 * @param c Corpo 1
 * @param p Corpo 2
 */
void colisao (game_object_type *c, game_object_type *p){
    vetor2d_type c_v_ini;

    c_v_ini = c->speed;

    c->body.speed = sqrt(p->body.mass * p->body.speed * p->body.speed + p->body.mass * p->body.speed * p->body.speed / c->body.mass);
    p->body.speed = sqrt(c->body.mass * c_v_ini.speed * c_v_ini.speed + c->body.mass * c->body.speed * c->body.speed / p->body.mass);
}


//função atrito
void atrito (game_object_type *a, float coefatrito, float dt){
    a->body.speed.x = a->body.speed.x+(coefatrito * GRAVIDADE) * (dt);
}
