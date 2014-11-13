#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<iostream>
using namespace std;

#define LEFT   	75
#define RIGHT  	77
#define UP     	72
#define DOWN   	80
#define ESC    	27

int colisao(int px, int py, int praio, int ox1, int ox2, int oy1, int oy2)
{
    if ((px-praio <= ox2 && px+praio >= ox1) && (py-praio <=oy2 && py+praio >= oy1))
       return 1;
    else
       return 0;
}

int main()
{
  int driver, mode;
  char tecla;
  int  x, y, passo, passox, passoy, raio, colide=0;

  tecla = 0;
  x = 100;
  y = 100;
  raio = 18; 
  passo = 5; // incremento do centro do circulo
  passox = passo;
  passoy = passo;

  detectgraph(&driver, &mode);
  initgraph(&driver, &mode, "");
  cleardevice();
  
  while(tecla != ESC) {
    setfillstyle(1, BLACK);
    setcolor (BLACK);
    fillellipse(x, y, raio, raio);

    fflush(stdin);
    if (kbhit()){
      tecla = getch();
      if(tecla == LEFT)  x = x - passox;
      if(tecla == RIGHT) x = x + passox;
      if(tecla == UP)    y = y - passoy;
      if(tecla == DOWN)  y = y + passoy;
    }
    
    setfillstyle(1, YELLOW);
    setcolor (YELLOW);
    fillellipse(x, y, raio, raio);
    delay(10);
    
    colide = colisao(x, y, raio, 150, 200, 300, 350);
       
    if (colide == 1){
       setcolor (GREEN);
       setfillstyle(1, GREEN);
       bar(150, 300, 200, 350);
    }
    if (colide == 0){
       setcolor (BLUE);
       setfillstyle(1, BLUE);
       bar(150, 300, 200, 350);
    }
    
    
  }

  closegraph();
  return(0);
}
