#include <iostream>
#include <graphics.h>
#include <math.h>
#include "grafico.h"

int print(vetor2d_type pos, vetor2d_type vel, vetor2d_type screenmax, int *groundoffset){

	//cleardevice();
	
	if(pos.x>=SCREEN_CENTER)
		*groundoffset = *groundoffset-vel.x;
		
	setcolor(BLUE);
	setfillstyle(1,BLUE);
	bar(0,screenmax.y-10,screenmax.x,screenmax.y);
	

	for(int i =0; i<=2*(screenmax.x/100);i++){
		int posground=i*100+*groundoffset;
		if(i%2>0){
			setcolor(CYAN);
			setfillstyle(1,CYAN);
			bar(posground,screenmax.y-10,posground+100,screenmax.y);

		}				
	}
	
	

	setcolor(WHITE);
	setfillstyle(1,WHITE);
	fillellipse(pos.x, pos.y, 20,20);
	

	
	if(*groundoffset <=-200)
		*groundoffset = 0;	
	

	
}

void erase(vetor2d_type pos){
	setcolor(BLACK);
	setfillstyle(1,BLACK);
	fillellipse(pos.x, pos.y, 20,20);
	
}



