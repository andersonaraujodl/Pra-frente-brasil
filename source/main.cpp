/**
 * @brief Arquivo main para inicializações e menu(?)
 *
 * @date 10/10/14
 * @author Anderson, Carolina, Diego, Lucas e Marcelo. Do grupo mais fodão
 * @version 1.0
 */


#include<iostream>
#include "bgi/graphics.h"
#include <time.h>

int main(int argc, char** argv) {

	srand (time(NULL));
	
	while(gameStates());

	while(!kbhit()); // espera até alguma tecla	se pressiona
	closegraph(); // fecha janela gráfica
	return 0;
}


/**
 *  @brief Carrega os recursos necessários em memória
 */
void loadResorces (void){

}
