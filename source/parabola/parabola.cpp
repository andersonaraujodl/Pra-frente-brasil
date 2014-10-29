#include <iostream>
#include<math.h>
#include<graphics.h>

#define g -10
#define t 0.1

using namespace std;

struct xy{
    float x;
    float y;
    float vx;
    float vy;
};

float radianos(float ang)
{
    return (ang/180)*M_PI;
}

struct xy salto(struct xy pos)    //da um salto
{
	pos.vx *= cos(radianos(45));
  	pos.vy = fabs(pos.vy)*sin(radianos(45));
  	return(pos);
}

struct xy mudaVel(struct xy pos, float fatorVel)    //Muda Velocidade
{
	pos.vx *= fatorVel;
	pos.vy *= fatorVel;
  	return(pos);
}

struct xy queda(struct xy pos)    //Queda do objeto
{
	pos.vx = 0;
    pos.vy = 0;
  	return(pos);
}

struct xy voo(float x, float y, float vx, float vy, float ang, int acao)
{
    struct xy pos;
    
    x=x+(vx*t);
    y=y+(vy*t)+(g*(t*t))/2;

    pos.x=x;
    pos.y=y;
    pos.vx=vx;
    pos.vy=vy;

    if (pos.y<=0)    //quique
    {
        pos.y=0;
        pos.vy = (fabs(pos.vy)*sin(radianos(ang)))*0.7;
    }
    else
    {
        pos.vy += (g*t);
    }

	if (acao==1)  //da um salto
 	{
  	   pos = salto(pos);
	}
    if (acao==2)   //dobra velocidade
    {
       pos = mudaVel(pos, 2);
    }
    if (acao==3)   //bate em bloco e cai
    {
       pos = queda(pos);
    }
    if (acao==4)   //bate em bloco, quebra e perde velocidade
    {
       mudaVel(pos, 0.5);
    }

    return(pos);
}


int main()
{
    float ang,vel;
    struct xy pos;
    int 
    cont=0, teste=0;

    cout<<"Angulo: ";
    cin>>ang;
    cout<<"Velocidade: ";
    cin>>vel;

    pos.x=0;
    pos.y=0;

    pos.vx = vel*cos(radianos(ang));
    pos.vy = vel*sin(radianos(ang));
    
    initwindow(600,600);
    
    do{
        moveto(int(pos.x), int(pos.y));
        pos = voo(pos.x,pos.y,pos.vx,pos.vy,ang,teste);
		
		if (pos.y==0)
			cont++;
		else
			cont=0;
		
        lineto(int(pos.x),int(pos.y));
        cout<<"x: "<<pos.x<<" e y: "<<pos.y;
        cin>>teste;
        
    }while(cont<4);

    system("pause");
}

