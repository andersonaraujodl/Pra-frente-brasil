/**
*  @file game.cpp
*  @brief Arquivo conténdo toda a lógica do game e interface com a BGI
*  
*  @author
*  @version 0.1
*  @date 10/10/14
*/ 

#include "h/types.h"
#include "graphs/graficos.h"

#define MAX_OBSTACLES 300
 
game_object_type player1,player2;
game_object_type world_obstacles[MAX_OBSTACLES];

graph_data_type graphs_profiles[NUM_OBJECTS_DEFINE];

typedef struct{
	char file_name[20];
	int heigth;
	int width;
}init_file_type;


/**
 *  @brief Inicia os recursos que serão utilzados no game
 *  
 *  
 *  @details Details
 */ 
void initGame (void){
	// FAZER LOAD DO ARQUIVO COM OS NOMES DAS IMAGENS ==========
	init_file_type init_file[] = {
		{"../resources/player1.gif",10,10},
		{"../resources/nuvem.gif",10,10},
		{"../resources/igreja.gif",10,10}
	};
	// =========================================================
	
	for(int i = 0; i < 3 /*subistituir 3 por NUM_OBJECTS_DEFINE*/;++i){
	
		graphs_profiles[i].h = init_file[i].heigth;
		graphs_profiles[i].w = init_file[i].width;
		graphs_profiles
	
	
		graphInitObject(&graphs_profiles[i],init_file[i].file_name);
	}
	
	player1.graphs = graphs_profiles[PLAYER].graphs;
	
}

void initObstacles (){
	
	for(int i = 0; i < MAX_OBSTACLES;++i){
		// Coloca cada objeto subdividindo cada espaço em dezenas, deixando aleatório a casa das unidades.
		// O primeiro objeto é colocado apartir da primeira dezena e assim por diante (1x, 2x, 3x, ...)
		world_obstacles[i].body.pos.x = ((rand() % 10) + ( (i+1) * 10));
		// A priori todos os objetos estarão no chão
		world_obstacles[i].body.pos.y = 0;
		
		// Randomiza qual o perfil (tipo) de obstáculo ele será, Igreja, nuvem....
		int obj_profile = (rand() % NUM_BLOCKS);
		world_obstacles[i].collision_mask = obj_profile;
		world_obstacles[i].graph.img = bitmaps[i]; // Aponta para qual o bitmap que pertence aquele perfil
	}
}


void loadMultiGame (void){

}


void connectToServer (void){

}

void gameStep (void){
	
	
}

void loadSingleGame (void){
	
	player1.body.pos.x = 0;
	player1.body.pos.y = 0;
	
// TESTE =============================================	
// Depois do teste iniciar com 0,0 e mudar depois com a opção do jogador
	player1.body.speed.setVector(50,45);
//====================================================
	player1.graphs.img = bitmaps[PLAYER];

	initObstacles();
	
	
}


void rodadaSingle (){
	
	loadSingleGame();

}

enum{
	MENU_INICIAL,
	MENU_COMPRA,
	JOGO_SINGLE,
	JOGO_MULTI,
	SCORE_FINAL
};


bool gameStates(){
	static int state = MENU_INICIAL;
	static prev_state = MENU_INICIAL;
	static int rodadas;
	
	switch(states){
		case MENU_INICIAL:
			switch(printMenu())
			{
				case 0:
					loadSingleGame();
					rodadas = 5;
					state = JOGO_SINGLE;
					prev_state = MENU_INICIAL;
				break;
				case 1:
					loadMultServer();
					rodadas = 5;
					state = JOGO_MULTI;
					prev_state = MENU_INICIAL;
				break;
				case 2:
					loadMultClient();
					rodadas = 5;
					state = JOGO_MULTI;
					prev_state = MENU_INICIAL;
				break;
				
				case 3:
					return false;
			}
			
		return true;
		
		case MENU_COMPRA:
			showMenuCompra();
			
			state = prev_state;
		return true;
		
		case JOGO_SINGLE:
			
			if(--rodadas){
				while(rodadaSingle());
			
				state = MENU_COMPRA;
				prev_state = JOGO_SINGLE;
				return true;
			}
			
			state = SCORE_FINAL;
	
		return true;
	
	
	
	}

	
}


