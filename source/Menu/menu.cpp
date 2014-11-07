#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<iostream>
using namespace std;

#define ESC    	27

/**
 * @brief Fun��o escolha de op��es do menu
 * @param Vetor de quatro elementos (op��es)
 * @return op��o clicada
 */
int main(){

  int driver, mode;
  char tecla;
  int  x, y;
  POINT P;
  HWND hwnd;

  tecla = 0;
  detectgraph(&driver, &mode);
  initgraph(&driver, &mode, "");

  hwnd = GetForegroundWindow(); 
  
  cleardevice();
  setcolor (LIGHTRED);
  rectangle(10, 10, 400, 400); //desenha ret�ngulo menu  (n�o sei se este item � necess�rio)
  setfillstyle(1, YELLOW); //define a cor de preenchimento do ret�ngul
  setcolor (YELLOW); //define a cor da borda do ret�ngulo
  
  rectangle(20, 20, 190, 190); //desenha ret�ngulo 1
  
  rectangle(220, 20, 390, 190); //desenha ret�ngulo 2
  
  rectangle(20, 220, 190, 390); //desenha ret�ngulo 3
  
  rectangle(220, 220, 390, 390); //desenha ret�ngulo 4
  
  while(tecla!= ESC){
	if(GetKeyState(VK_LBUTTON)&0x80) { //clique do mouse
        if (GetCursorPos(&P)) //retorna uma estrutura que cont�m a posi��o atual do cursor
          if (ScreenToClient(hwnd, &P))  //converte as coordenadas tela de um apontar especificado na tela para coordenadas client
            if ((P.x >= 20 && P.x <= 190) && (P.y >= 20 && P.y<= 190)){
                setfillstyle(1, GREEN); //define a cor de preenchimento do ret�ngul
                setcolor (GREEN);
                rectangle(20, 20, 190, 190); //desenha ret�ngulo 1
            	//return 1;
             }
            if ((P.x >= 220 && P.x <= 390) && (P.y >= 20 && P.y<= 190)){
                setfillstyle(1, WHITE);
                setcolor (WHITE);
                rectangle(220, 20, 390, 190); //desenha ret�ngulo 1
            	//return 2;
             }
            if ((P.x >= 20 && P.x <= 190) && (P.y >= 220 && P.y<= 390)){
                setfillstyle(1, LIGHTBLUE);
                setcolor (LIGHTBLUE);
                rectangle(20, 220, 190, 390); //desenha ret�ngulo 1
                //return 3;
             }
            if ((P.x >= 220 && P.x <= 390) && (P.y >= 220 && P.y<= 390)){
                setfillstyle(1, LIGHTGREEN);
                setcolor (LIGHTGREEN);
                rectangle(220, 220, 390, 390); //desenha ret�ngulo 1
            	//return 4;
             }
		}
	fflush(stdin);    
    if (kbhit())       tecla = getch();
}
  closegraph();

} 

    
