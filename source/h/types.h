/**
 *  @file types.h
 *  @brief Arquivo conténdo as definições comuns aos demais módulos
 *  do projeto
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <math.h>

/**
* Macro para facilitar as contas utilizando o @ref NUM_OBJECTS_DEFINE.
* Exemplo: if(player.collision_mask & MASK_BIT(MST)){...}
*/
#define MASK_BIT(x) (long int)( 1 << x)



#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

/**
 *  Vetor2D básico
 */
typedef struct vetor2d_type{
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
	
	void sum(vetor2d_type *vec){
		x+= vec->x;
		y+= vec->y;
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
enum{
/*	MIDIA,
	CLASSE_MEDIA,
	MONTADORAS,
*/
	FEMINISTAS,
	LEGALIZACAO,
	CONGRESSO,
	SAUDE,
	NUVEM_POLUICAO,
	IGREJA,
	LGBT,
	BANCO,
	INDUSTRIA,
	RURALISTAS,
	MTST,
	GROUND,
	PLAYER1,
	PLAYER2,
	PLAYERS_MASK,
	NUM_OBJECTS_DEFINE

};

/**
 *  Quantidade de Objetos - o player
 */
#define NUM_BLOCKS (NUM_OBJECTS_DEFINE -5)

#define ON_DEBUG

void debugTrace (char *msg){
#ifdef ON_DEBUG
	std::cout << "debugTrace: " << msg << std::endl;
#endif	
}




#endif
