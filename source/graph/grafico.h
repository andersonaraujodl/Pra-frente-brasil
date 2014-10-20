#ifndef _FORMULAS_H__
#define _FORMULAS_H__

#define SCREEN_W 1024
#define SCREEN_H 600
#define SCREEN_CENTER SCREEN_W/2


// Declaração de tipos
typedef struct{
	float y;
	float x;
	
}vetor2d_type;

/*typedef struct{
	void *img;
	vetor2d_type pos;
	float h;
	float w;
	vetor2d_type vel; 
}graph_data_type;*/


// Protótipo das funções ------------------------------
int print(vetor2d_type pos, vetor2d_type vel, vetor2d_type screenmax, int *groundoffset);
void erase(vetor2d_type pos);

#endif
