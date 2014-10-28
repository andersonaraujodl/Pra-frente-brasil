/**
 *  @file grafico.h
 *  @brief Arquivo contendo as definições gráficas utilizadas no projeto
 */

#ifndef _FORMULAS_H__
#define _FORMULAS_H__

#include "../h/types.h"

/**
	 *  @brief Definições globais da tela
	 */

#define SCREEN_W 1024 /**< Largura da tela*/
#define SCREEN_H 600 /**< Altura da tela*/
#define SCREEN_CENTER SCREEN_W/2 /**< centro horizontal da tela*/
#define PLAYER_SPOT SCREEN_W/3 /**< Posição horizontal da tela que será o ponto fixo do personagem*/
#define BGCOLOR BGCOLOR COLOR(100,50,50) /**< cor de fundo da tela*/


/*typedef struct{
	void *img;
	vetor2d_type pos;
	float h;
	float w;
	vetor2d_type vel;
}graph_data_type;*/


// Protótipo das funções ------------------------------
void updateScreen();
void print(graph_data_type *objeto);
void erase();
void graphInitObjects(graph_data_type *objeto, const char* caminho);
void printBG(graph_data_type *player, graph_data_type *background, vetor2d_type screenmax);

#endif
	