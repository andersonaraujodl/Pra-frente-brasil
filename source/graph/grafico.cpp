#include "grafico.h"
#include "../bgi/graphics.h"
#include<iostream>


#define TO_GROUND_LEVEL(y) ((SCREEN_H-10)- y)

using namespace std;


/**
 *  @brief Brief
 *  
 *  @param [in] pos  Parameter_Description
 *  @param [in] obj  Parameter_Description
 *  @param [in] mode Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
void print(vetor2d_type pos, graph_data_type *obj, int mode){
	float ref_y = pos.y + obj->h;
	
	if(obj->msk) putimage(pos.x, TO_GROUND_LEVEL(ref_y), obj->msk, AND_PUT);
	
	putimage(pos.x, TO_GROUND_LEVEL(ref_y), obj->img, mode);
	
}

/**
 *  @brief Brief
 *  
 *  @return Return_Description
 *  
 *  @details Details
 */
void erase(){
	cleardevice();
	setbkcolor(BGCOLOR);
	
}

/**
 *  @brief Brief
 *  
 *  @param [in] objeto  Parameter_Description
 *  @param [in] caminho Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
void graphInitObjects(graph_data_type *objeto, const char* caminho, const char* caminhomsk){
	unsigned size;
	int left, top, right, bottom;
	setbkcolor(BGCOLOR);
	
	int page = getactivepage();
	setactivepage(2);
	
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
	
	if(strcmp(caminhomsk," ")){
		readimagefile(caminhomsk,left, top, right, bottom );
		size= imagesize(left, top,right,bottom);
		objeto->msk = new int[size];
		getimage(left, top, right, bottom, objeto->msk);
		cleardevice();	
	}
	
	setvisualpage(page);
}

/**
 *  @brief Brief
 *  
 *  @param [in] pos    Parameter_Description
 *  @param [in] angulo Parameter_Description
 *  @param [in] forca  Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
void printDirection(vetor2d_type pos,float angulo, float forca){
	vetor2d_type temp = {pos.x, pos.y };
	temp.setVector (forca/4, angulo);
	setwritemode(XOR_PUT);
	setlinestyle(0,0,3);
	setcolor(YELLOW);
	line(pos.x, TO_GROUND_LEVEL(pos.y), pos.x+temp.x, TO_GROUND_LEVEL(pos.y)-temp.y);
}

/**
 *  @brief Brief
 *  
 *  @return Return_Description
 *  
 *  @details Details
 */
void updateScreen(){
	int page = getactivepage();
	setvisualpage(page);
	
	page = page ? 0:1;
	setactivepage(page);
	erase();
}


void printTxt(char *texto, vetor2d_type pos){
	setcolor(WHITE);
	outtextxy(pos.x, pos.y,texto);	
}
