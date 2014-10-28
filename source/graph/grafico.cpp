#include <iostream>
#include <graphics.h>
#include <math.h>
#include "grafico.h"


void print(graph_data_type *objeto){
	
	putimage(objeto->pos.x-objeto->w, objeto->pos.y-objeto->h, objeto->img, OR_PUT);
	
}

void erase(){
	cleardevice();
	setbkcolor(BGCOLOR);
	
}


void printBG(graph_data_type *objeto, graph_data_type *background, vetor2d_type screenmax){
	if(objeto->pos.x>= PLAYER_SPOT)
		background->pos.x = background->pos.x-objeto->vel.x;
		
	if(background->pos.x <=(float)-background->w)
		background->pos.x = background->pos.x + background->w;
	
	for(int i =0; i<=(screenmax.x/background->w)+1;i++){
		int posground=i*background->w+background->pos.x;
		putimage(posground, background->pos.y, background->img, COPY_PUT);
	}	
}

void graphInitObjects(graph_data_type *objeto, const char* caminho){
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
void updateScreen(){
	int page = getactivepage();
	setvisualpage(page);
	
	page = page ? 0:1;
	setactivepage(page);
	
}

