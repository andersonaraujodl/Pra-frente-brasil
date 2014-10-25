#ifndef _FORMULAS_H__
#define _FORMULAS_H__

#include "../h/types.h"

#define SCREEN_W 1024
#define SCREEN_H 600
#define SCREEN_CENTER SCREEN_W/2
#define PLAYER_SPOT SCREEN_W/3
#define BGCOLOR RED


/*typedef struct{
	int type;
	void *img;
	vetor2d_type pos;
	float h;
	float w;
	vetor2d_type vel;
}graph_data_type;*/


// Protótipo das funções ------------------------------
void print(graph_data_type *player);
void erase(graph_data_type *player);
void clear();
void initObjects(graph_data_type *player);
void printBG(graph_data_type *player, graph_data_type *background, vetor2d_type screenmax);

#endif
	