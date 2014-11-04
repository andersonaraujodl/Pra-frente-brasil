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


#define RESOURCES_ROOT "../resources/mimimi.txt"

int main(int argc, char** argv) {
	
	int ret = 0;

	srand (time(NULL));
	
	while(ret >= 0){
		
		
		ret = game_states->func(1000/30);
		
		
		game_states = game_states->states[ret];
		
	
		delay(1000/30);
	}

	while(!kbhit()); // espera até alguma tecla	se pressiona
	closegraph(); // fecha janela gráfica
	return 0;
}


/**
 *  @brief Carrega os recursos necessários em memória
 */
void loadResorces (void){

}
