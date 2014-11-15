/**
 *  @file types.h
 *  @brief Arquivo cont�ndo as defini��es comuns aos demais m�dulos
 *  do projeto
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <math.h>
#include <iostream>
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
	void *msk; /**< Imagem que ser� usada como mascara*/
	bool masked;
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
	
	vetor2d_type bottomLeft() {return this->body.pos;}
	vetor2d_type topRight() { return vetor2d_type{this->body.pos.x + this->graph.w,this->body.pos.y + this->graph.h};}
	
	
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
	COMUNICACAO,
	
	MTST, // Deixar esse por �ltimo dos blocks
	
	GROUND,
	PLAYER1,
	PLAYER2,
	RED_AURA,
	GREEN_AURA,
	LOGOTIPO,
	MENU_OPTION_1,
	MENU_OPTION_2,
	MENU_OPTION_3,
	MENU_OPTION_4,
	
	//imagens antes do NUM_OBJECTS_DEFINE
	NUM_OBJECTS_DEFINE

};

/**
 *  Quantidade de Objetos - o player
 */
#define NUM_BLOCKS (MTST +1)
#define NUM_OPTIONS_MENU 4
#define ON_DEBUG

#endif
