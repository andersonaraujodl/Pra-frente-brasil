#ifndef _GRAFICOS_H__
#define _GRAFICOS_H__

#include "../h/types.h"
#include "../bgi/graphics.h"
//definições das  propriedades de tela
#define SCREEN_W 1024
#define SCREEN_H 600
#define SCREEN_CENTER SCREEN_W/2
#define PLAYER_SPOT SCREEN_W/3
#define BGCOLOR COLOR(131,169,196)

//definições de IDs pros objetos gráficos utilizados para definir o que deve ser desenhado no initobj
//pode ser útil também na hora de cálcular as posições dos ítens
	
// Protótipo das funções ------------------------------
void updateScreen();
void print(vetor2d_type pos, graph_data_type *obj, int mode = COPY_PUT);
void erase();
void graphInitObjects(graph_data_type *objeto, const char* caminho, const char* caminhomsk="");
void printDirection(vetor2d_type pos,float angulo, float forca);
void printTxt(char *texto, vetor2d_type pos);
void fontSize(int char_size=1);

#endif
