#ifndef _FORMULAS_H__
#define _FORMULAS_H__

#define SCREEN_W 1024
#define SCREEN_H 600
#define SCREEN_CENTER SCREEN_W/2


// Declara��o de tipos
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


// Prot�tipo das fun��es ------------------------------
int print(vetor2d_type pos, vetor2d_type vel, vetor2d_type screenmax, int *groundoffset);
void erase(vetor2d_type pos);

#endif
