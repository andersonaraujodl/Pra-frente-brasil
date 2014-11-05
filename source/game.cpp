/**
*  @file game.cpp
*  @brief Arquivo cont�ndo toda a l�gica do game e interface com a BGI
*  
*  @author
*  @version 0.1
*  @date 10/10/14
*/ 

#include "game.h"
#include "graph/grafico.h"
#include "physics/physics.h"
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <string.h>
#include <fstream>
// Defines =====================================================
#define MAX_OBSTACLES 300 /**< N�mero m�ximo de obst�culos que ser�o gerados por partida.*/
#define PLAYER_FIX_POS PLAYER_SPOT /**< Posi��o m�xima que o player avan�ar� em rela��o a tela*/

#define PLAYER_INIT_X 10 /**< Posi��o incial do player em x*/
#define PLAYER_INIT_Y 0  /**< Posi��o incial do player em y*/
#define RESOURCES_ROOT "../resources/data/img_data.cvs"


// typedefs privados ===========================================
typedef struct{
	char *file_name;
	int heigth;
	int width;
}init_file_type;


// Prot�tipos Privados ==========================================
int initGame (float dt);
int showMenu (float dt);
int loadSingleGame (float dt);
int preLancamento (float dt);
int singleStep (float dt);
//Vari�veis privadas ============================================
game_object_type player1,player2;
game_object_type world_obstacles[MAX_OBSTACLES];

graph_data_type graphs_profiles[NUM_OBJECTS_DEFINE];
unsigned int left_obstacles_index = 0,right_obstacles_index = 0;

game_state_type load_state ={
	initGame,
	(game_state_type*[]){
		&load_state,		 // return 0
		&menu_state		 	 // return 1
	}
};
game_state_type menu_state ={
	showMenu,
	(game_state_type*[]){
		&menu_state,		 // return 0
		&load_single/*, 	 // return 1
		conectaServer,		 // return 2	
		aguardaClient,		 // return 3
		&terminaPrograma*/ // return 4
	}
};

game_state_type load_single ={
	loadSingleGame,
	(game_state_type*[]){
		&load_single,	 // return 0
		&pre_lancamento // return 1
	},
	1
};

game_state_type pre_lancamento ={
	preLancamento,
	(game_state_type*[]){
		&pre_lancamento,	// return 0
		&step_single 		// return 1
	},
	1
};

game_state_type step_single ={
	singleStep,
	(game_state_type*[]){
		&step_single,	 // return 0
		&pre_lancamento  // return 1
	},
	1
};

game_state_type *game_states =  &load_state;

/**
 *  @brief Inicia os recursos que ser�o utilzados no game
 *  
 *  
 *  @details Details
 */ 
int initGame (float dt){
	// FAZER LOAD DO ARQUIVO COM OS NOMES DAS IMAGENS ==========
	using namespace std;	
	ifstream file(RESOURCES_ROOT);
	
	char data[1000];
	int pos =0;
	while(file.good()){
		data[pos] = file.get();
		pos++;
	}
	data[pos] = '\0';
	
	char * pch;
		
	pch = strtok(data,",");
	
	for(int i = 0; i <NUM_OBJECTS_DEFINE; ++i){
		static char temp[200], t_width[4], t_height[5];
		strcpy(temp,pch);
		cout<<temp<<endl;
		pch = strtok(NULL,",");
		strcpy(t_height,pch);
		graphs_profiles[i].h = atoi(t_height);
		cout<<graphs_profiles[i].h<<endl;
		pch = strtok(NULL,"\n\0");
		strcpy(t_width,pch);
		graphs_profiles[i].w = atoi(t_width);
		cout<<graphs_profiles[i].w<<endl;
		pch = strtok(NULL,",");
		
		graphInitObjects(&graphs_profiles[i], temp);
	}
	player1.graph = graphs_profiles[PLAYER1];
	return 1;
}

int showMenu (float dt){
	std::cout<<"showMenu"<<std::endl;
	erase();
	print(vetor2d_type{SCREEN_W/2, SCREEN_H/2}, player1.graph.img);
	return 0;
}

void initObstacles (void){
	
/*	for(int i = 0; i < MAX_OBSTACLES;++i){
		// Coloca cada objeto subdividindo cada espa�o em dezenas, deixando aleat�rio a casa das unidades.
		// O primeiro objeto � colocado apartir da primeira dezena e assim por diante (1x, 2x, 3x, ...)
		world_obstacles[i].body.pos.x = ((rand() % 10) + ( (i+1) * 10));
		// A priori todos os objetos estar�o no ch�o
		world_obstacles[i].body.pos.y = 0;
		
		// Randomiza qual o perfil (tipo) de obst�culo ele ser�, Igreja, nuvem....
		int obj_profile = (rand() % NUM_BLOCKS);
		world_obstacles[i].collision_mask = obj_profile;
		world_obstacles[i].graph.img = graphs_profiles[obj_profile].img; // Aponta para qual o bitmap que pertence aquele perfil
	}*/
}

void loadMultiGame (void){

}


void connectToServer (void){

}

bool atualizaObjetos (game_object_type &ref){
	
	
/*	float ref_x = (ref.body.pos.x < PLAYER_FIX_POS) ? ref.body.pos.x: PLAYER_FIX_POS;
	
	print(vetor2d_type{ref_x,ref.body.pos.y},ref.graph.img);
	
	// Enquanto o objeto.pos.x mais a esquerda for menor que o canto mais a esquerda da tela
	while(world_obstacles[left_obstacles_index].body.pos.x < ref.body.pos.x - PLAYER_FIX_POS){
		if(++left_obstacles_index == MAX_OBSTACLES)
			return false;// impede que estoure o buffer
	}
	// Enquanto o objeto mais a direita (+1) for menor que o canto direito da tela
	while(world_obstacles[right_obstacles_index+1].body.pos.x < ref.body.pos.x +(SCREEN_W - PLAYER_FIX_POS)){
		if(++right_obstacles_index == MAX_OBSTACLES)
			return false; // Impede que estoure o buffer
	}
	
	// Imprime todos aqueles que est�o dentro do range da tela
	for (int i = left_obstacles_index; i <= right_obstacles_index; ++i){
		float obj_x = world_obstacles[i].body.pos.x -  ref.body.pos.x + PLAYER_FIX_POS;
		print(vetor2d_type{obj_x,world_obstacles[i].body.pos.y},world_obstacles[i].graph.img);
	
	}*/
}

/**
*	@brief Inicializa as condi��es para um novo jogo posicionando
*  o player no local certo e gerando um novo mapa.
*	@param dt Nessa fun��o n�o h� utilidade a n�o ser compatibilidade com os demais states
*
*   @return 1 para passar para o pr�ximo estado, -1 em caso de erro
*/
int loadSingleGame (float dt){
	
/*	player1.body.pos.x = PLAYER_INIT_X;
	player1.body.pos.y = PLAYER_INIT_Y;
	
	player1.body.speed.setVector(0,0);

	player1.graph.img = graphs_profiles[PLAYER1].img;

	initObstacles();
	
	return 1; // next state = preLancamento*/
}

/**
*	@brief Estado no qual o jogo aguarda o jogador escolher o �ngulo e 
*	o momento em que ele deseja lan�ar o player1.
*
*	@param dt delta tempo para a atualiza��o do demonstrativo do �ngulo escolhido
*	@return 0 caso o jogador ainda n�o tenha feito a escolha, 1 para quando deve-se iniciar o jogo.
*/
int preLancamento (float dt){
/*	int key;
	key = kbhit();
	
	player1.body.speed.setVector(50,45);
		
	atualizaObjetos(player1);
		
	if(key) 
		return 1; // singleStep
	else
		return 0; // preLancamento*/
}
/**
*	@brief Estado no qual o jogo est� rodando. 
*
*	@param dt delta tempo para o c�lculo do espa�o percorrido.
*	@return 0 para o jogo em andamento, 1 para quando a velocidade seja igual a 0.
*/
int singleStep (float dt){
	
/*	lancamento(&player1,dt);
	
	atualizaObjetos(player1);
	
	if(player1.body.speed.modulo())
		return 0;     //singleStep
	else
		return 1;*/
}

