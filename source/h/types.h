/**
 *  @file types.h
 *  @brief Arquivo cont�ndo as defini��es comuns aos demais m�dulos
 *  do projeto
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <math.h>

/**
* Macro para facilitar as contas utilizando o @ref block_type.
* Exemplo: if(player.collision_mask & MASK_BIT(MST)){...}
*/
#define MASK_BIT(x) (long int)( 1 << x)


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
		return (((x<<2) + (y<<2)) >> 2); // sqrt( x^2 + y^2 )
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
	
	void sum(vetor2d_type &vec){
		x+= vec.x;
		y+= vec.y;
	};
}vetor2d_type;

/**
*	Estrutura de manipula��o dos objetos do jogo,
*  reunindo todas as propriedades b�sicas para tal.
*/
typedef struct 
{
	physics_data_type body; /**< Propriedades f�sicas*/
	graph_data_type graph; /**< Propriedades gr�ficas*/
	long int collision_mask; /**< M�scara para a an�lise de colis�es entre objetos*/
}game_object;

/**
* Enumera��o dos bloqueios encontrados durante o jogo
* 
* @todo Definir os bloqueios finais
*/
typedef enum{
	NUVEM_POLUICAO,
	IGREJA,
	LGBT,
	MIDIA,
	BANCO,
	COMUNIDADES,
	CLASSE_MEDIA,
	RURALISTAS,
	MST,
	MONTADORAS
}block_type;







#endif
