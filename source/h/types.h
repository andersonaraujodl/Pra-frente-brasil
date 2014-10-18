/**
 *  @file types.h
 *  @brief Arquivo conténdo as definições comuns ao demais globais
 *  ao projeto
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <math.h>

/**
 *  Vetor2D básico
 */
typedef struct{
	float x; /**< Decomposição x do vetor*/
	float y; /**< Decomposição y do vetor*/
	
	/**
	 *  @brief Calcula o módulo de vetor
	 *  @return O valor do módulo
	 */
	float modulo(){
		return sqrt((x*x) + (y*y));
	}
	
	/**
	 *  @brief Inicializa o vetor utilizando módulo e angulo em graus
	 *  
	 *  @param [in] modulo Módulo do vetor
	 *  @param [in] angle  Angulo em graus do vetor em relação ao eixo x 
	 *  @return Return_Description
	 */
	void setVector (float modulo, float angle){
		angle = (angle*M_PI)/180;
		x = cos(angle)*modulo;
		y = sin(angle)*modulo;
	}
}vector2d_type;







#endif
