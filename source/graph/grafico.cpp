#include "grafico.h"
#include "../bgi/graphics.h"
#include<iostream>


#define TO_GROUND_LEVEL(y) ((SCREEN_H-10)- y)

using namespace std;

void print(vetor2d_type pos, graph_data_type *obj, int mode= COPY_PUT){
	float ref_y = pos.y + obj->h;
	putimage(pos.x, TO_GROUND_LEVEL(ref_y), obj->img, mode);
	
}

void erase(){
	cleardevice();
	setbkcolor(BGCOLOR);
	
}




void graphInitObjects(graph_data_type *objeto, const char* caminho){//puxar arquivo com caminho e tamanhos das imagens 
	unsigned size;
	int left, top, right, bottom;
	setbkcolor(BGCOLOR);
	cleardevice();
	left = (SCREEN_W/2)-(objeto->w/2);
	top = (SCREEN_H/2)-(objeto->h/2);
	right = (SCREEN_W/2)+(objeto->w/2);
	bottom = (SCREEN_H/2)+(objeto->h/2);	
	readimagefile(caminho,left, top, right, bottom );
	
	size= imagesize(left, top,right,bottom);
	objeto->img = new int[size];
	getimage(left, top, right, bottom, objeto->img);
	
	cleardevice();
	
	
}

void printDirection(vetor2d_type pos,float angulo, float forca){
	vetor2d_type temp = {pos.x, pos.y };
	temp.setVector (forca/4, angulo);
	setwritemode(XOR_PUT);
	setlinestyle(0,0,3);
	setcolor(YELLOW);
	line(pos.x, TO_GROUND_LEVEL(pos.y), pos.x+temp.x, TO_GROUND_LEVEL(pos.y)-temp.y);
}

void updateScreen(){
	int page = getactivepage();
	setvisualpage(page);
	
	page = page ? 0:1;
	setactivepage(page);
	erase();
}
