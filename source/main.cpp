/**
 * @brief Arquivo main para inicializa��es e menu(?)
 *
 * @date 10/10/14
 * @author Anderson, Carolina, Diego, Lucas e Marcelo. Do grupo mais fod�o
 * @version 1.0
 */


#include<iostream>
#include "bgi/graphics.h"
#include <time.h>

int main(int argc, char** argv) {

	srand (time(NULL));
	
	while(gameStates());

	while(!kbhit()); // espera at� alguma tecla	se pressiona
	closegraph(); // fecha janela gr�fica
	return 0;
}


/**
 *  @brief Carrega os recursos necess�rios em mem�ria
 */
void loadResorces (void){

}
