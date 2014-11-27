#include "grafico.h"
#include "../bgi/graphics.h"
#include<iostream>


#define TO_GROUND_LEVEL(y) ((SCREEN_H-10)- y)



using namespace std;


/**
 *  @brief Imprime na tela imagem contida no graph_dta_type passado, se tiver máscara faz o uso dela para simular transparência
 *  
 *  @param [in] pos  Posição do objeto (só será impresso se estiver dentro dos limites da tela @see SCREEN_H @see SCREEN_W
 *  @param [in] obj  Objeto a ser impresso
 */
void print(vetor2d_type pos, graph_data_type *obj){
	int mode = COPY_PUT;
	float ref_y = pos.y + obj->h;
	
	if(obj->msk){
	 	putimage(pos.x, TO_GROUND_LEVEL(ref_y), obj->msk, AND_PUT);
	 	mode = OR_PUT;
	 }
	
	putimage(pos.x, TO_GROUND_LEVEL(ref_y), obj->img, mode);
	
}

/**
 *  @brief Limpa a page corrente (não necessariamente a aparete)
 */
void erase(){
	cleardevice();
	setbkcolor(BGCOLOR);
}

/**
 *  @brief Faz a carga de uma imagem apartir do path do arquivo para a memória do programa
 *  
 *  @param [put] objeto  Onde a imagem será estocada
 *  @param [in] caminho Path da imagem
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
	
	setactivepage(page);
}

/**
 *  @brief Imprime a linha referência de lançamento assim como a barra de força
 *  
 *  @param [in] pos    posição referência
 *  @param [in] angulo Angulo da linha em relação ao chão
 *  @param [in] forca  formça em %  
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
 *  @brief Faz a comutação entre a página de escrita com a página visual.
 *  
 */
void updateScreen(){
	int page = getactivepage();
	setvisualpage(page);
	
	page = page ? 0:1;
	setactivepage(page);
	erase();
}

/**
 *  @brief Define o tamanho da fonte da letras a serem impressas
 *  
 *  @param [in] char_size  tamanho da fonte
 */
void fontSize(int char_size){
		settextstyle(4,HORIZ_DIR, char_size);
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
void printTxt(char *texto, vetor2d_type pos){ 
	outtextxy(pos.x, pos.y,texto);	
}

/**
 */
void drawProgressBar(float value, vetor2d_type pos){
	float r = 0, g = 0, b = 255;
	int fase = 0, fases = 4;
	
	
	float step = 255/((BAR_MAX_HEIGHT/BAR_UNIT)/fases);
	setwritemode(COPY_PUT);

	for(int i = 0; i <= value/BAR_UNIT; ++i  ){
		switch(fase){
			
			case 0:
				g = g + step;
				if(g>255){
					g=255;
					fase =1;
				} 
				break; 
				
			case 1:
				b = b - step;
				if(b<0){
					b=0;
					fase =2;
				} 
				break; 
			case 2:
				r = r+step;
				if(r>255){
					r=255;
					fase =3;
				} 
				break; 
			
			case 3:
				g = g-step;
				if(g<0){
					r=0;
					fase =4;
				} 
				break; 
			case 4:
				break; 		
						
		}
		setfillstyle(1,COLOR(r,g,b));
		bar(pos.x, TO_GROUND_LEVEL(i*BAR_UNIT+pos.y), pos.x+BAR_WIDTH, TO_GROUND_LEVEL(i*BAR_UNIT+pos.y+BAR_UNIT));
	}
}
