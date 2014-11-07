/**
 * @brief Arquivo main para inicializações e loop principal do jogo
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

		
		ret = game_states->func(TARGET_FRAME_RATE);
		
		
		game_states = game_states->states[ret];
		
		updateScreen();
		delay(TARGET_FRAME_RATE * 1000.0);
	}

	while(!kbhit()); // espera até alguma tecla	se pressiona
	closegraph(); // fecha janela gráfica
	return 0;
}



