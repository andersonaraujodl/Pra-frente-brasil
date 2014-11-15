#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<iostream>
using namespace std;

#define ESC    	27
#define SCREEN_W 1280
#define SCREEN_H 600
#define NUM_LOJA_MENU 20

/**
 * @brief Função escolha de opções do menu
 * @param Vetor de quatro elementos (opções)
 * @return opção clicada
 */
int main(){

  char tecla, texto[50];
  int  menux[21], menuy[21], j, i, prim_select=0, segun_select=0;
  POINT P;
  HWND hwnd;

  tecla = 0;
  initwindow(SCREEN_W,SCREEN_H);
  
  hwnd = GetForegroundWindow(); 
  
  cleardevice();
  setfillstyle(1, YELLOW); //define a cor de preenchimento do retângul
  setcolor (YELLOW); //define a cor da borda do retêngulo
  
  for(j=1;j <= NUM_LOJA_MENU;j++){
              if (j<=10) menux[j] = 30; else menux[j] = 530;
              if (j == 1 || j == 11) menuy[j] = 20; else menuy[j] = menuy[j-1] + 40;
              rectangle(menux[j], menuy[j], menux[j]+400, menuy[j]+30); //desenha retângulo
              sprintf(texto,"Proposta %d",j);
              outtextxy(menux[j]+30, menuy[j]+5,texto);
  }
  
  menux[0] = 1010;
  menuy[0] = 20;
  rectangle(menux[0], menuy[0], menux[0]+100, menuy[0]+50); //desenha retângulo
  sprintf(texto,"Selecionar");
  outtextxy(menux[0]+20, menuy[0]+10,texto);
  
  while(tecla!= ESC){
	for(int i = 0; i <= NUM_LOJA_MENU; ++i){
		if(GetKeyState(VK_LBUTTON)&0x80){ //clique do mouse
			POINT mouse_pos;
			if (GetCursorPos(&mouse_pos)){ //retorna uma estrutura que contém a posição atual do cursor
				char debug[50];
								
				HWND hwnd = GetForegroundWindow(); 
				if (ScreenToClient(hwnd, &mouse_pos)){
					
					// Passa o y para as coordenadas de nossos objetos
					//mouse_pos.y = SCREEN_H - mouse_pos.y;
					// Verifica os limites de x
					if((mouse_pos.x < menux[i]) || (mouse_pos.x > menux[i]+400)) continue;
					// Verifica os limites de y
					if((mouse_pos.y < menuy[i]) || (mouse_pos.y > menuy[i]+30)) continue;
					// Retorna a opção do menu			
					
					if(i == 0){
						if (prim_select > 0 && segun_select > 0){ //return 1;    // se tiver 2 itens selecionados vai para o jogo
						sprintf(texto,"Propostas selecionadas: %d  %d",prim_select,segun_select);
                        outtextxy(1010, 150,texto);}
					}
					else
					if(prim_select == 0 && segun_select != i){   //verifica se item não está selecionado e seleciona como primeiro
						prim_select = i;
						setfillstyle(1, GREEN); //define a cor de preenchimento do retângul
                        setcolor (GREEN); //define a cor da borda do retêngulo
                        rectangle(menux[i], menuy[i], menux[i]+400, menuy[i]+30); //desenha retângulo
//						print(menu_loja_selected[i].body.pos,&menu_loja_selected[i].graph);
					}
					else if(prim_select == i){   //verifica se item está selecionado como primeiro e tira a seleção
						prim_select = 0;
						setfillstyle(1, YELLOW); //define a cor de preenchimento do retângul
                        setcolor (YELLOW); //define a cor da borda do retêngulo
                        rectangle(menux[i], menuy[i], menux[i]+400, menuy[i]+30); //desenha retângulo
//						print(menu_loja[i].body.pos,&menu_loja[i].graph);
					}
					else if(prim_select != 0 && segun_select == 0){   //verifica se item não está selecionado e seleciona como segundo
						segun_select = i;
						setfillstyle(1, GREEN); //define a cor de preenchimento do retângul
                        setcolor (GREEN); //define a cor da borda do retêngulo
                        rectangle(menux[i], menuy[i], menux[i]+400, menuy[i]+30); //desenha retângulo
//						print(menu_loja_selected[i].body.pos,&menu_loja_selected[i].graph);
					}
					else if(segun_select == i){   //verifica se item está selecionado como segundo e tira a seleção
						segun_select = 0;
						setfillstyle(1, YELLOW); //define a cor de preenchimento do retângul
                        setcolor (YELLOW); //define a cor da borda do retêngulo
                        rectangle(menux[i], menuy[i], menux[i]+400, menuy[i]+30); //desenha retângulo
//						print(menu_loja[i].body.pos,&menu_loja[i].graph);
					}
				}
			}
			delay(100);
		}
	}	
	fflush(stdin);    
    if (kbhit())       tecla = getch();
}
  closegraph();

} 

    
