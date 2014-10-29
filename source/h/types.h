/**
 *  @file types.h
 *  @brief Arquivo conténdo as definições comuns aos demais módulos
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
		return sqrt(((x*x) + (y*y)));
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
	
	void sum(vetor2d_type &vec){
		x+= vec.x;
		y+= vec.y;
	};
}vetor2d_type;

/**
* Carol preencher a doc aqui
*/
typedef struct {
    vetor2d_type speed; /**< Carol, explica o que é isso*/
    vetor2d_type pos;   /**< Carol, explica o que é isso*/
    float mass;
} physics_data_type;

/**
* Estrutura de manipulação gráfica dos objetos
*/
typedef struct{
	void *img; /**< Imagem que será impressa*/
	float h; /**< Altura da imagem*/
	float w; /**< Largura da imagem*/
}graph_data_type;

/**
*	Estrutura de manipulação dos objetos do jogo,
*  reunindo todas as propriedades básicas para tal.
*/
typedef struct 
{
	physics_data_type body; /**< Propriedades físicas*/
	graph_data_type graph; /**< Propriedades gráficas*/
	long int collision_mask; /**< Máscara para a análise de colisões entre objetos*/
}game_object_type;

/**
* Enumeração dos bloqueios encontrados durante o jogo
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
