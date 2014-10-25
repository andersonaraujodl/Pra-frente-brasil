#include <iostream>
#include <graphics.h>
#include <math.h>
#include "grafico.h"


void drawGraph(graph_data_type *graph_obj, int *left, int *top, int *right, int *bottom);

void print(graph_data_type *objeto){
	
	putimage(objeto->pos.x-20, objeto->pos.y-20, objeto->img, COPY_PUT);
	
}

void erase(graph_data_type *objeto){
	setcolor(BGCOLOR);
	setfillstyle(1,BGCOLOR);
	bar(0,0,SCREEN_W, SCREEN_H-10);
	//fillellipse(objeto->pos.x, objeto->pos.y, objeto->w*2, objeto->h*2);
	setbkcolor(BGCOLOR);
	
}
void clear(){
	cleardevice();
}

void printBG(graph_data_type *objeto, graph_data_type *background, vetor2d_type screenmax){
	if(objeto->pos.x>= PLAYER_SPOT)
		background->pos.x = background->pos.x-objeto->vel.x;
		
	if(background->pos.x <=(float)-200)
		background->pos.x = background->pos.x + 200;
	
	for(int i =0; i<=(screenmax.x/background->w)+1;i++){
		int posground=i*background->w+background->pos.x;
		putimage(posground, background->pos.y, background->img, COPY_PUT);
	}	
}

void initObjects(graph_data_type *graph_obj){
	unsigned size;
	int left, top, right, bottom;
	
	
	drawGraph(graph_obj, &left, &top, &right, &bottom);
	
	size= imagesize(left, top,right,bottom);
	graph_obj->img = new int[size];
	getimage(left, top, right, bottom, graph_obj->img);
	
	
	cleardevice();
	
	
}

//função que vai desenhar o objeto a ser transformado em bitmap
void drawGraph(graph_data_type *graph_obj, int *left, int *top, int *right, int *bottom){
	
	setbkcolor(BGCOLOR);
	cleardevice();
	if(graph_obj->type == PLAYER){
	
		setcolor(WHITE);
		setfillstyle(1,WHITE);
		fillellipse(SCREEN_W/2, SCREEN_H/2, graph_obj->w ,graph_obj->h);
		*left = (SCREEN_W/2)-(graph_obj->w);
		*top = (SCREEN_H/2)-(graph_obj->h);
		*right = (SCREEN_W/2)+(graph_obj->w);
		*bottom = (SCREEN_H/2)+(graph_obj->h);
	
	} else if(graph_obj->type == BACKGROUND){
		setcolor(BLUE);
		setfillstyle(1,BLUE);
		bar((SCREEN_W/2)-(graph_obj->w/2), (SCREEN_H/2)-(graph_obj->h/2),(SCREEN_W/2)+(graph_obj->w/2),(SCREEN_H/2)+(graph_obj->h/2));
		setcolor(CYAN);
		setfillstyle(1,CYAN);
		bar(SCREEN_W/2,(SCREEN_H/2)-(graph_obj->h/2),(SCREEN_W/2)+(graph_obj->w/2),(SCREEN_H/2)+(graph_obj->h/2));
		*left = (SCREEN_W/2)-(graph_obj->w/2);
		*top = (SCREEN_H/2)-(graph_obj->h/2);
		*right = (SCREEN_W/2)+(graph_obj->w/2);
		*bottom = (SCREEN_H/2)+(graph_obj->h/2);
		
	} else if(graph_obj->type == ITENS){
		setcolor(GREEN);
		setfillstyle(1,GREEN);
		fillellipse(SCREEN_W/2, SCREEN_H/2, graph_obj->w ,graph_obj->h);
		*left = (SCREEN_W/2)-(graph_obj->w);
		*top = (SCREEN_H/2)-(graph_obj->h);
		*right = (SCREEN_W/2)+(graph_obj->w);
		*bottom = (SCREEN_H/2)+(graph_obj->h);
	}
}

