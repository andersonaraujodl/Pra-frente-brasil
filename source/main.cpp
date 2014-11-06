/**
 * @brief Arquivo main para inicializa��es e loop principal do jogo
 *
 * @date 10/10/14
 * @author Anderson, Carolina, Diego, Lucas e Marcelo.
 * @version 1.0
 */


#include<iostream>
#include "bgi/graphics.h"
#include <time.h>
#include "game.h"
#include "h/types.h"
#include "graph/grafico.h"






int main(int argc, char** argv) {
	
	int ret = 0;

	srand (time(NULL));
	initwindow(SCREEN_W,SCREEN_H);
	
	while(ret >= 0){
		//if(kbhit()) keyCapture();
		
		ret = game_states->func((float)1/30);
		
		
		game_states = game_states->states[ret];
		
		updateScreen();
		delay(1000/30);
	}

	while(!kbhit()); // espera at� alguma tecla	se pressiona
	closegraph(); // fecha janela gr�fica
	return 0;
}



