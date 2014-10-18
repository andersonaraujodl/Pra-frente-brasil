/**
 *  @file types.h
 *  @brief Arquivo cont�ndo as defini��es comuns ao demais globais
 *  ao projeto
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <math.h>

/**
 *  Vetor2D b�sico
 */
typedef struct{
	float x; /**< Decomposi��o x do vetor*/
	float y; /**< Decomposi��o y do vetor*/
	
	/**
	 *  @brief Calcula o m�dulo de vetor
	 *  @return O valor do m�dulo
	 */
	float modulo(){
		return sqrt((x*x) + (y*y));
	}
	
	/**
	 *  @brief Inicializa o vetor utilizando m�dulo e angulo em graus
	 *  
	 *  @param [in] modulo M�dulo do vetor
	 *  @param [in] angle  Angulo em graus do vetor em rela��o ao eixo x 
	 *  @return Return_Description
	 */
	void setVector (float modulo, float angle){
		angle = (angle*M_PI)/180;
		x = cos(angle)*modulo;
		y = sin(angle)*modulo;
	}
}vector2d_type;







#endif
