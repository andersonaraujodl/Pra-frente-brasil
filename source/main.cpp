/**
 * @brief Arquivo main para inicializações e menu(?)
 *
 * @date 10/10/14
 * @author Anderson, Carolina, Diego, Lucas e Marcelo. Do grupo mais fodão
 * @version 1.0
 */


#include<iostream>
#include "bgi/graphics.h"

int main(int argc, char** argv) {
	
	initwindow(400, 300); //abre uma janela gráfic	a de 400X300 pixels




	while(!kbhit()); // espera até alguma tecla	se pressiona
	closegraph(); // fecha janela gráfica
	return 0;
}


/**
 *  @brief Carrega os recursos necessários em memória
 */
void loadResorces (void){

}
