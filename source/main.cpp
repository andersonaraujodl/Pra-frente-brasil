/**
 * @brief Arquivo main para inicializa��es e menu(?)
 *
 * @date 10/10/14
 * @author Anderson, Carolina, Diego, Lucas e Marcelo. Do grupo mais fod�o
 * @version 1.0
 */


#include<iostream>
#include "bgi/graphics.h"

int main(int argc, char** argv) {
	
	initwindow(400, 300); //abre uma janela gr�fic	a de 400X300 pixels




	while(!kbhit()); // espera at� alguma tecla	se pressiona
	closegraph(); // fecha janela gr�fica
	return 0;
}


/**
 *  @brief Carrega os recursos necess�rios em mem�ria
 */
void loadResorces (void){

}
