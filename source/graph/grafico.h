#ifndef _FORMULAS_H__
#define _FORMULAS_H__

#include "../h/types.h"

#define SCREEN_W 1024
#define SCREEN_H 600
#define SCREEN_CENTER SCREEN_W/2


/*typedef struct{
	void *img;
	float height;
	float width;
}graph_data_type;*/


// Protótipo das funções ------------------------------
int print(vetor2d_type pos, vetor2d_type vel, vetor2d_type screenmax, int *groundoffset);
void erase(vetor2d_type pos);

#endif
