#ifndef _GRAFICOS_H__
#define _GRAFICOS_H__

#include "../h/types.h"
//definições das  propriedades de tela
#define SCREEN_W 1024
#define SCREEN_H 600
#define SCREEN_CENTER SCREEN_W/2
#define PLAYER_SPOT SCREEN_W/3
#define BGCOLOR COLOR(100,50,50)

//definições de IDs pros objetos gráficos utilizados para definir o que deve ser desenhado no initobj
//pode ser útil também na hora de cálcular as posições dos ítens
#define BACKGROUND 0 // o objeto do cenario ( podendo ser composto por uma ou mais camadas 
#define PLAYER 1 //não preciso explicar, né? :P
#define OPONENT 2 //AKA player2
#define ITENS 3 //barreiras
	
// Protótipo das funções ------------------------------
void updateScreen();
void print(vetor2d_type pos, void *img);
void erase();
void graphInitObjects(graph_data_type *objeto, const char* caminho);

#endif