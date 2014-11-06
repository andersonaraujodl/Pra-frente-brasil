#include <iostream>
#include "physics.h"


/**
 * @brief Fun��o de convers�o de graus em radianos
 * @param Valor do angulo em graus
 * @return valor do angulo em radianos
 */
float radianos(float ang){
    return (ang/180)*M_PI;
}


/**
 * @brief Calcula a nova posi��o do objeto em trajet�ria obliqua (sem altera��o em x)
 * @param p Corpo sobre a qual a nova posi��o ser� calculada
 * @param dt delta tempo
 */
void lancamento(game_object_type *p, float dt){
    p->body.pos.x = p->body.pos.x +(p->body.speed.x*dt);
    p->body.speed.y = p->body.speed.y +GRAVIDADE;
	p->body.pos.y = p->body.pos.y + (p->body.speed.y*dt) +(GRAVIDADE*(dt*dt))/2 ; 
}

/**
 * @brief Calcula o efeito da colis�o sobre os corpos
 * @param c Corpo 1
 * @param p Corpo 2
 */
void colisao (game_object_type *c, game_object_type *p){
}


//fun��o atrito
void atrito (game_object_type *a, float coefatrito, float dt){
    a->body.speed.x *= (coefatrito * dt);
}
