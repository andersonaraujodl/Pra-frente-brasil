/**
 *  @file types.h
 *  @brief Arquivo cont�ndo as defini��es comuns aos demais m�dulos
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
 *  Vetor2D b�sico
 */
typedef struct vetor2d_type{
	float x; /**< Decomposi��o x do vetor*/
	float y; /**< Decomposi��o y do vetor*/
	
	/**
	 *  @brief Calcula o m�dulo de vetor
	 *  @return O valor do m�dulo
	 */
	float modulo(){
		return sqrt(((x*x) + (y*y)));
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
	
	void sum(vetor2d_type *vec){
		x+= vec->x;
		y+= vec->y;
	};
}vetor2d_type;

/**
* Carol preencher a doc aqui
*/
typedef struct {
    vetor2d_type speed; /**< Carol, explica o que � isso*/
    vetor2d_type pos;   /**< Carol, explica o que � isso*/
    float mass;
} physics_data_type;

/**
* Estrutura de manipula��o gr�fica dos objetos
*/
typedef struct{
	void *img; /**< Imagem que ser� impressa*/
	float h; /**< Altura da imagem*/
	float w; /**< Largura da imagem*/
}graph_data_type;

/**
*	Estrutura de manipula��o dos objetos do jogo,
*  reunindo todas as propriedades b�sicas para tal.
*/
typedef struct 
{
	physics_data_type body; /**< Propriedades f�sicas*/
	graph_data_type graph; /**< Propriedades gr�ficas*/
	long int collision_mask; /**< M�scara para a an�lise de colis�es entre objetos*/
}game_object_type;

/**
* Enumera��o dos bloqueios encontrados durante o jogo
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
