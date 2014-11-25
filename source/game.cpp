/**
*  @file game.cpp
*  @brief Arquivo conténdo toda a lógica do game e interface com a BGI
*  
*  @author
*  @version 0.1
*  @date 10/10/14
*/ 

#include "game.h"
#include "graph/grafico.h"
#include "physics/physics.h"
#include "socket/socket.h"
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <string.h>
#include <fstream>

// Defines =====================================================
#define MAX_OBSTACLES 300 /**< Número máximo de obstáculos que serão gerados por partida.*/
#define PLAYER_FIX_POS PLAYER_SPOT /**< Posição máxima que o player avançará em relação a tela*/
#define PLAYER_INIT_X 10 /**< Posição incial do player em x*/
#define PLAYER_INIT_Y 0  /**< Posição incial do player em y*/
#define RESOURCES_ROOT "../resources/data/img_data.cvs"
#define SERVER_CONF_PATH "../resources/data/server.inf"

#define ATRITO 50
#define BOUNCE 0.5
#define SPEED_LIM_X 2000
#define SPEED_LIM_Y 2000
#define MAX_BONUS 0.2

// typedefs privados ===========================================
typedef struct{
	char *file_name;
	int heigth;
	int width;
}init_file_type;


// Protótipos Privados ==========================================
int initGame (float dt);
int initMenu (float dt);
int showMenu (float dt);
int showLoja (float dt);
int initServer (float dt);
int serverSendObstacles (float dt);
int initClient (float dt);
int clientGetObstacles (float dt);
int loadSingleGame (float dt);
int setLaunchVector (game_object_type &ref);
int preLancamento (float dt);
int preLancamentoMult (float dt);
int singleStep (float dt);
int multiStep (float dt);
void floorCheck(game_object_type *player);
void groundStep(game_object_type *objeto, game_object_type *ground, float dt);
int singleEnd(float dt);
int showCredits (float dt);
float variaForca(float valor);
int initLoja (float dt);
void resetGame();
void resetLoja();

//Variáveis privadas ============================================
game_object_type player1,player2, ground;
game_object_type green_aura, red_aura;
game_object_type loja_options[NUM_LOJA_MENU];
game_object_type menu_options[NUM_OPTIONS_MENU];
game_object_type world_obstacles[MAX_OBSTACLES];
graph_data_type graphs_profiles[NUM_OBJECTS_DEFINE];
unsigned int left_obstacles_index = 0,right_obstacles_index = 0;
float  ground_offset;
float obstacles_weight[NUM_BLOCKS],max_obstacles_per_type[NUM_BLOCKS]; 
int total_obstacles = 0;
int total_score = 0; 
int total_rounds = 5;
float profile_collision_bonus[NUM_BLOCKS];
/**
*  
*/
game_state_type load_state ={
	initGame,
	(game_state_type*[]){
		&load_state,		 // return 0
		&load_menu_state	 // return 1
	}
};
/**
*  
*/
game_state_type load_menu_state ={
	initMenu,
	(game_state_type*[]){
		&load_menu_state, // return 0
		&menu_state       // return 1
	}
};
/**
*  
*/
game_state_type menu_state ={
	showMenu,
	(game_state_type*[]){
		&menu_state,		 // return 0
		&load_single, 	 // return 1
		&client_conect,		 // return 2	
		&server_conect,		 // return 3
		&termina_programa // return 4
	}
};
/**
*  
*/
game_state_type load_single ={
	loadSingleGame,
	(game_state_type*[]){
		&load_single,	 // return 0
		&pre_lancamento  // return 1
	}
};
/**
*  
*/
game_state_type pre_lancamento ={
	preLancamento,
	(game_state_type*[]){
		&pre_lancamento,	// return 0
		&step_single 		// return 1
	},
};
/**
*  
*/
game_state_type loja_state ={
	showLoja,
	(game_state_type*[]){
		&loja_state,	 // return 0
		&load_single	 // return 1
	}
};
/**
*  
*/
game_state_type load_loja ={
	initLoja,
	(game_state_type*[]){
		&load_loja,	 // return 0
		&loja_state	 // return 1
	}
};


game_state_type end_single ={
	singleEnd,
	(game_state_type*[]){
		&end_single,	 // return 0
		&load_loja,	 // return 1
		&load_menu_state // return 2
	}
};


/**
*  
*/
game_state_type step_single ={
	singleStep,
	(game_state_type*[]){
		&step_single,	 // return 0
		&end_single     // return 1
	}
};
/**
*  
*/
game_state_type termina_programa ={
	showCredits,
	(game_state_type*[]){
		&termina_programa,
		&load_menu_state
	}
};
/**
*  
*/
game_state_type server_conect ={
	initServer,
	(game_state_type*[]){
		&server_conect,
		&server_obstacles
	}
};
/**
*  
*/
game_state_type client_conect ={
	initClient,
	(game_state_type*[]){
		&client_conect,
		&client_obstacles
	}
};
/**
*  
*/
game_state_type server_obstacles ={
	serverSendObstacles,
	(game_state_type*[]){
		&server_obstacles,
		&server_prelancamento
	}
};
/**
*  
*/
game_state_type client_obstacles ={
	clientGetObstacles,
	(game_state_type*[]){
		&client_obstacles,
		&client_prelancamento
	}
};

game_state_type server_prelancamento ={
	preLancamentoMult,
	(game_state_type*[]){
		&server_prelancamento,
		&step_server
	}
};

game_state_type client_prelancamento ={
	preLancamentoMult,
	(game_state_type*[]){
		&client_prelancamento,
		&step_client
	}
};

game_state_type step_server ={
	multiStep,
	(game_state_type*[]){
		&step_server,	 // return 0
		&end_server     // return 1
	}
};

game_state_type step_client ={
	multiStep,
	(game_state_type*[]){
		&step_client,	 // return 0
		&end_client     // return 1
	}
};

game_state_type end_server ={
	singleEnd,
	(game_state_type*[]){
		&end_server,	 // return 0
		&load_loja_server,	 // return 1
		&load_menu_state // return 2
	}
};

game_state_type end_client ={
	singleEnd,
	(game_state_type*[]){
		&end_client,	 // return 0
		&load_loja_client,	 // return 1
		&load_menu_state // return 2
	}
};

game_state_type load_loja_server ={
	initLoja,
	(game_state_type*[]){
		&load_loja_server,	 // return 0
		&loja_server	 // return 1
	}
};

game_state_type load_loja_client ={
	initLoja,
	(game_state_type*[]){
		&load_loja_client,	 // return 0
		&loja_client	 // return 1
	}
};

game_state_type loja_server ={
	showLoja,
	(game_state_type*[]){
		&loja_server,	 // return 0
		&server_obstacles	 // return 1
	}
};

game_state_type loja_client ={
	showLoja,
	(game_state_type*[]){
		&loja_client,	 // return 0
		&client_obstacles	 // return 1
	}
};
/**
*  
*/
game_state_type *game_states =  &load_state;

static void debugTrace (char *msg){
#ifdef ON_DEBUG
	std::cout << "debugTrace: " << msg << std::endl;
#endif	
};

/**
 *  @brief Inicia os recursos que serão utilzados no game
 *  
 *  @param dt
 */ 
int initGame (float dt){
	
	// FAZER LOAD DO ARQUIVO COM OS NOMES DAS IMAGENS ==========
	using namespace std;	
	int f_index =0;
	char data[8000];
	
	{// Truque para deixar o tempo de vida do file mínima
		ifstream file(RESOURCES_ROOT);
		while(file.good()){
			data[f_index] = file.get();
			f_index++;
		}
		file.close();
	}
	data[f_index] = '\0';
	
	char * pch;
		
	pch = strtok(data,",");//carrega o primeiro booleano
	
	for(int i = 0; i <NUM_OBJECTS_DEFINE; ++i){
		static char temp[300],tempmsk[300], t_width[4], t_height[5];
		

		
		//carrega valor da largura
		strcpy(t_width,pch);
		graphs_profiles[i].w = atoi(t_width);
	

		//carrega valor da altura
		pch = strtok(NULL,",");
		strcpy(t_height,pch);
		graphs_profiles[i].h = atoi(t_height);
		
		//carrega caminho da mascara

		pch = strtok(NULL,",");
		strcpy(tempmsk,pch);	
	

		pch = strtok(NULL,"\n\0");
		strcpy(temp,pch);
		
		pch = strtok(NULL,",");
		
		// Carrega e captura a imagem na memória
		graphInitObjects(&graphs_profiles[i], temp,tempmsk);
	}
	// Inicializa o gráfico do player1
	player1.graph = graphs_profiles[PLAYER1];
	
	
	
	

	
	// Inicializa o gráfico do player2
	player2.graph = graphs_profiles[PLAYER2];
	
	// Parametros do ground
	ground.graph = graphs_profiles[GROUND];
	ground.body.pos.x = 0;
	ground.body.pos.y = -10 ;
	ground.body.speed.x = 0;
	ground.body.speed.y = 0;
	
	// Itens do menu
	for(int i = 0; i < NUM_OPTIONS_MENU; ++i){
		menu_options[i].graph = graphs_profiles[MENU_OPTION_1 + i];
		
		vetor2d_type menu_pos{(SCREEN_W - menu_options[i].graph.w)/2,(SCREEN_H/(NUM_OPTIONS_MENU +3)) * ( NUM_OPTIONS_MENU -1 - i)};
		menu_options[i].body.pos =	menu_pos;
	}
	
	resetGame();


	
	// Feedback visuais de colisão
	green_aura.graph = graphs_profiles[GREEN_AURA];
	red_aura.graph = graphs_profiles[RED_AURA];
	
	//carrega lista de pesos dos objetos
	for(int i = 0; i < NUM_BLOCKS; i++) obstacles_weight[i] = 1;
	
	f_index =0;
	{// Truque para deixar o tempo de vida do file mínima
		ifstream file(SERVER_CONF_PATH);
		while(file.good()){
			data[f_index] = file.get();
			f_index++;
		}
		file.close();
	}
	data[f_index] = '\0';
	
	char *ip = strtok(data,":");
	pch = strtok(NULL,":");
	short port = strtoul(pch,0,10);
	
	setServerConfig(ip,port);
	
	
	return 1;
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int showCredits (float dt){
	
	setcolor(COLOR(255,255,255));
	fontSize(2);
	printTxt("Obrigado por jogar!", vetor2d_type{(SCREEN_W/2)-(textwidth("Obrigado por jogar!")/2), SCREEN_H/2-(textheight("Obrigado por jogar!")+20)});
	
	printTxt("Anderson Araújo", vetor2d_type{(SCREEN_W/2)-(textwidth("Anderson Araújo")/2), SCREEN_H/2-(textheight("Anderson Araújo"))});
	printTxt("Carol Fernandes", vetor2d_type{(SCREEN_W/2)-(textwidth("Carol Fernandes")/2), SCREEN_H/2-(textheight("Carol Fernandes")-20)});
	printTxt("Diego Hortiz", vetor2d_type{(SCREEN_W/2)-(textwidth("Diego Hortiz")/2), SCREEN_H/2-(textheight("Diego Hortiz")-40)});
	printTxt("Lucas Pina", vetor2d_type{(SCREEN_W/2)-(textwidth("Lucas Pina")/2), SCREEN_H/2-(textheight("Lucas Pina")-60)});
	printTxt("Marcelo Pietragala", vetor2d_type{(SCREEN_W/2)-(textwidth("Marcelo Pietragala")/2), SCREEN_H/2-(textheight("Marcelo Pietragala")-80)});
	
	if(kbhit())
		return -1;
		
	return 0;
}

/**
 *  @brief Limpa a memória heap
 */
void endGame (void){
	
	for(int i = 0; i <NUM_OBJECTS_DEFINE; ++i){
		delete graphs_profiles[i].img;
	}
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int initMenu (float dt){
	
	if(kbhit()){
		getch();
		fflush(stdin);
	}
	// Pura graça -------------------------------
	player1.body.pos.y = (SCREEN_H*3)/4;
	player1.body.pos.x = (SCREEN_W/6);
	player2.body.pos.setVector((SCREEN_W*5)/6,0);
	player2.body.speed.setVector(750,90);
	// ------------------------------------------
	return 1;
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int showMenu (float dt){
	
	// Pura graça -------------------------------
	lancamento(&player1,dt);
	lancamento(&player2,dt);
	// Verifica se atingiu o chão
	if(player1.body.pos.y<=0){
		player1.body.pos.y=0;
		player1.body.speed.y *= -0.99;
	}
	// Verifica se atingiu o chão
	if(player2.body.pos.y<=0){
		player2.body.pos.y=0;
		player2.body.speed.y *= -0.99;
	}
	print(vetor2d_type{0,-10},&graphs_profiles[LOGOTIPO]);
	print(player1.body.pos,&player1.graph);
	print(player2.body.pos,&player2.graph);
	// -------------------------------------------
	
	
	for(int i = 0; i < NUM_OPTIONS_MENU; ++i)
		print(menu_options[i].body.pos,&menu_options[i].graph);

	for(int i = 0; i < NUM_OPTIONS_MENU; ++i){
		if(GetKeyState(VK_LBUTTON)&0x80){ //clique do mouse
			POINT mouse_pos;
			if (GetCursorPos(&mouse_pos)){ //retorna uma estrutura que contém a posição atual do cursor
				char debug[50];
								
				HWND hwnd = GetForegroundWindow(); 
				if (ScreenToClient(hwnd, &mouse_pos)){
					
					// Passa o y para as coordenadas de nossos objetos
					mouse_pos.y = SCREEN_H - mouse_pos.y;
					// Verifica os limites de x
					if((mouse_pos.x < (int)menu_options[i].bottomLeft().x) || (mouse_pos.x > (int)(menu_options[i].topRight().x))) continue;
					// Verifica os limites de y
					if((mouse_pos.y < (int)menu_options[i].bottomLeft().y) || (mouse_pos.y > (int)menu_options[i].topRight().y)) continue;
					// Retorna a opção do menu			
					return i + 1;
				}
			}
		}
	}	
	return 0;
}
/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int initLoja (float dt){
		
	for(int i = 0; i < NUM_BLOCKS;++i)	profile_collision_bonus[i] = 0.0;// reseta o bonus
	
	if(kbhit()){
		getch();
		fflush(stdin);
	}

	return 1;
}
int showLoja (float dt){
	
	static int selected = 0;
	int i, a=0;
	char texto[50];
	static game_object_type *moving_object = 0;
	static game_object_type *moving_pairobject = 0;
	const int pairpolitics[] = {1,0,3,2,5,4,7,6,9,8,11,10,13,12,-1,-1,-1,-1,-1,-1,-1,-1};
	const float bonus[NUM_LOJA_MENU-1][NUM_BLOCKS] = {	{0.0,	1.0,	0.0,	0.5,	0.0,	-1.0,	0.0,	0.5,	0.0,	0.0,	0.0,	-1.0,	0.0,	0.0,	0.0},
														{0.0,	-1.0,	0.0,	0.0,	0.0,	0.5,	0.0,	0.0,	0.0,	0.0,	0.0,	1.0,	0.0,	0.0,	0.0},
														{0.0,	0.0,	0.0,	0.5,	0.5,	0.0,	0.0,	-1.0,	0.0,	-1.0,	0.0,	0.0,	0.0,	0.5,	1.0},
														{0.0,	0.0,	0.0,	-0.5,	2.0,	0.0,	0.0,	1.0,	0.0,	1.0,	0.0,	0.0,	0.0,	-1.0,	-1.0},
														{-0.5,	-0.5,	-0.5,	0.0,	0.0,	0.0,	-0.5,	1.0,	1.0,	0.0,	1.0,	0.0,	0.0,	-0.5,	-0.5},
														{0.5,	0.5,	0.5,	0.0,	0.0,	0.0,	0.5,	-1.0,	-1.0,	0.0,	-1.0,	0.0,	0.0,	0.5,	0.5},
														{0.5,	0.5,	0.0,	0.0,	0.0,	0.0,	0.5,	-0.5,	0.0,	0.0,	0.0,	0.0,	-0.5,	0.5,	0.5},
														{-0.5,	-0.5,	0.0,	0.0,	0.0,	0.0,	-0.5,	0.5,	0.0,	0.0,	0.0,	0.0,	0.5,	-0.5,	-0.5},
														{0.5,	-0.5,	0.0,	0.0,	0.5,	0.0,	0.5,	0.0,	0.0,	0.0,	0.0,	0.0,	-0.5,	0.5,	0.0},
														{0.0,	-1.0,	0.0,	0.0,	0.0,	0.5,	0.0,	0.0,	0.0,	0.0,	0.0,	1.0,	0.0,	0.0,	0.0},
														{1.0,	1.0,	-1.0,	0.5,	0.0,	0.0,	1.0,	-0.5,	0.5,	0.0,	0.0,	0.0,	0.0,	1.0,	1.0},
														{-1.0,	-1.0,	-1.0,	-0.5,	0.0,	0.0,	-1.0,	0.5,	-0.5,	0.0,	0.0,	0.0,	0.0,	-1.0,	-1.0},
														{0.0,	0.5,	0.5,	0.0,	1.5,	0.0,	-0.5,	-1.0,	1.0,	1.0,	-1.0,	0.0,	0.0,	-0.5,	0.0},
														{0.0,	-0.5,	0.5,	0.0,	0.0,	0.0,	0.5,	1.0,	-1.0,	-1.0,	1.0,	0.0,	0.0,	0.0,	0.0},
														{0.5,	0.5,	-1.0,	0.0,	0.0,	0.0,	0.5,	-0.5,	0.0,	0.0,	0.0,	0.0,	-0.5,	0.5,	0.5},
														{-0.5,	-0.5,	0.0,	0.0,	0.0,	0.0,	-0.5,	1.0,	-1.0,	0.5,	1.0,	0.0,	0.0,	-0.5,	-0.5},
														{0.0,	0.0,	0.0,	1.0,	0.5,	0.0,	0.0,	0.0,	0.0,	-1.0,	0.0,	0.0,	0.0,	1.0,	0.0},
														{0.5,	0.5,	-0.5,	0.0,	0.0,	0.0,	0.5,	0.0,	0.0,	0.0,	0.0,	0.0,	-0.5,	0.5,	0.5},
														{1.0,	0.0,	0.0,	1.0,	0.0,	-1.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0},
														{1.0,	0.5,	-1.0,	0.0,	0.0,	-0.5,	1.0,	0.0,	0.0,	0.0,	-1.0,	0.0,	-1.0,	0.0,	0.5},
														{0.5,	0.0,	0.0,	0.0,	0.0,	-1.0,	1.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0},
														{1.0,	1.0,	-1.0,	0.0,	0.0,	0.0,	1.0,	-1.0,	0.0,	0.0,	-1.0,	0.0,	-0.5,	1.0,	1.0}
														};
												
												
	for(i=0;i < NUM_LOJA_MENU;i++)
        print(loja_options[i].body.pos,&loja_options[i].graph);

	if(moving_pairobject){
		moving_pairobject->body.pos.x -= 25;
		
		if( moving_pairobject->body.pos.x< (0 - moving_pairobject->graph.w)) moving_pairobject=0;

	}
	if(moving_object){
		moving_object->body.pos.x -= 25;
		
		if( moving_object->body.pos.x< (0 - moving_object->graph.w)) moving_object=0;
		
		return 0;
	}

	for(int i = 0; i <= NUM_LOJA_MENU; ++i){
		if(GetKeyState(VK_LBUTTON)&0x80){ //clique do mouse
			POINT mouse_pos;
			
			if (GetCursorPos(&mouse_pos)){ //retorna uma estrutura que contém a posição atual do cursor
				char debug[50];
				HWND hwnd = GetForegroundWindow(); 
				if (ScreenToClient(hwnd, &mouse_pos)){
					
					// Passa o y para as coordenadas de nossos objetos
					//mouse_pos.y = SCREEN_H - mouse_pos.y;
					mouse_pos.y = SCREEN_H - mouse_pos.y;
					// Verifica os limites de x
					if((mouse_pos.x < (int)loja_options[i].bottomLeft().x) || (mouse_pos.x > (int)loja_options[i].topRight().x)) continue;
					// Verifica os limites de y
					if((mouse_pos.y < (int)loja_options[i].bottomLeft().y) || (mouse_pos.y > (int)loja_options[i].topRight().y)) continue;
					// Retorna a opção do menu			
					if(i == LOJA_OK - LOJA_OPTION_1) {
						selected = 0;
						return 1;
					}
					if(i == LOJA_RESET - LOJA_OPTION_1) {
						resetLoja();
						selected = 0;
						return 0;
					}
					if(selected <2){
						++selected;					
						moving_object = &loja_options[i];
						if(pairpolitics[i]>=0) moving_pairobject = &loja_options[pairpolitics[i]];
						//carrega em profile_collision_bonus os valores selecionados
						for(int u = 0; u <NUM_BLOCKS ; ++u){
							profile_collision_bonus[u] = profile_collision_bonus[u]+bonus[i][u]*MAX_BONUS;
							if(u == NUVEM_POLUICAO) profile_collision_bonus[u] = -1* MAX_BONUS;
							
							obstacles_weight[u] = obstacles_weight[u]-(bonus[i][u]/4);
							
							
						}	
					}
				
					
					
				}
			}
		}
	}

	return 0;
} 

/**
 *  @brief Brief
 *  
 *  @return Return_Description
 *  
 *  @details Details
 */
void initObstacles (void){
	//carrega lista de números máximos de cada objeto
	for(int i = 0; i < NUM_BLOCKS; i++){
		
		if(i == CONGRESSO)
			max_obstacles_per_type[i] =1;
		else
	 		max_obstacles_per_type[i] = (MAX_OBSTACLES/NUM_BLOCKS) *obstacles_weight[i];
	 		
		total_obstacles+=max_obstacles_per_type[i];
	}
	
	//testa se o total de obstaculos previstos ultrapassa o numero maximo do sistema
	if(total_obstacles> MAX_OBSTACLES)
		total_obstacles= MAX_OBSTACLES;
	//	
	int obstacles_defined = 0;
	while(obstacles_defined < total_obstacles){

		// Randomiza qual o perfil (tipo) de obstáculo ele será, Igreja, nuvem....
		unsigned int obj_profile = (rand() % NUM_BLOCKS);
		
		//checa se o profile selecionado já alcançou o número limite previsto 
		//se sim, sorteia novamente
		//se não, segue com o procedimento de carregar ele na lista
		if(max_obstacles_per_type[obj_profile]>0){
		
				world_obstacles[obstacles_defined].profile = obj_profile;
				world_obstacles[obstacles_defined].graph = graphs_profiles[obj_profile]; // Aponta para qual o bitmap que pertence aquele perfil
				
				//partindo da posição minima para deixar fora da tela, a posição de cada obstaculo varia 100px,
				//a partir do primeiro objeto colocado a posicao minima vira a posição do objeto anterior
				//e a ele é somado a largura desse mesmo objeto objeto anterior, de forma a evitar a sobreposição
				world_obstacles[obstacles_defined].body.pos.x = ((rand() % 600) + 50) + ((obstacles_defined != 0) ? world_obstacles[obstacles_defined-1].graph.w + world_obstacles[obstacles_defined-1].body.pos.x: SCREEN_W - 50);
				
				if(obj_profile == NUVEM_POLUICAO || obj_profile== EUA)
					world_obstacles[obstacles_defined].body.pos.y = ((rand() % 300) + 50);
				else
					world_obstacles[obstacles_defined].body.pos.y = 0;
				
				max_obstacles_per_type[obj_profile]--;
				++obstacles_defined;
		}
	}
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int initServer (float dt){
	initSocket();
	
	char texto[] = "Wating for client";
	printTxt(texto, vetor2d_type{(SCREEN_W/2)-(textwidth(texto)/2), SCREEN_H/2});
	
	
	if(!waitClient()) return 0;
	
	return 1;
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int serverSendObstacles (float dt){

	static bool new_round = true;

	if(new_round){
		initObstacles();
		new_round = false;
	}
	
	char texto[] = "Loading Map";
	printTxt(texto, vetor2d_type{(SCREEN_W/2)-(textwidth(texto)/2), SCREEN_H/2});
	
	packet_type resp;
	if(getPacket(resp) > 0){
		if(resp.ctrl. operation == OBSTACLE_UPDATE){
			union{
				short i_16;
				char i_8[2];
			};
			
			// Converte o 8 bits em 16. (altamente inseguro devido os diferentes endiands entre máquinas
			i_8[0] = resp.buff[0];
			i_8[1] = resp.buff[1];
		
			// Preenche as informações sobre o obstaculo
			gam_obj_pack_type obst{
					world_obstacles[i_16].profile,
					world_obstacles[i_16].body.pos.x,
					world_obstacles[i_16].body.pos.y,
					0
			};
			// Prepara o packed de resposta
			packet_type resp{
				{
					0,
					(sizeof(gam_obj_pack_type)+2),
					OBSTACLE_UPDATE
				},
				{}
			};
			
			memcpy(&resp.buff[2],&obst,sizeof(gam_obj_pack_type));
			resp.buff[0] = i_8[0];
			resp.buff[1] = i_8[1];
			sendPacket(resp);	
		}
		else if(resp.ctrl. operation == WAINTING_GAME){
			player1.body.pos.x = PLAYER_INIT_X;
			player1.body.pos.y = PLAYER_INIT_Y;
			
			player2.body.pos.x = PLAYER_INIT_X;
			player2.body.pos.y = PLAYER_INIT_Y;
	
			return 1;
		}
	}
	return 0;
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int initClient (float dt){
	#define RELOAD_RETRY 0.8
	static float retry_time =0.0;
	initSocket();
		
	retry_time -= dt;
	if(retry_time <= 0){
		if(connectToServer()){
			debugTrace("Conected");
			return 1;
		}
		retry_time = RELOAD_RETRY;
	}
	
/*	char texto[] = "Searching Server";
	printTxt(texto, vetor2d_type{(SCREEN_W/2)-(textwidth(texto)/2), SCREEN_H/2});
	*/
	
	return 0;
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int clientGetObstacles (float dt){
	static bool new_round = true;
	
	if(new_round){
		debugTrace("clientGetObstacles: new_round");
		
		memset(world_obstacles,0,sizeof(game_object_type) * MAX_OBSTACLES);
		new_round = false;
	}
	
	char texto[] = "Loading Map";
	printTxt(texto, vetor2d_type{(SCREEN_W/2)-(textwidth(texto)/2), SCREEN_H/2});
	
	packet_type resp;
	if(getPacket(resp) > 0){
		if(resp.ctrl. operation == OBSTACLE_UPDATE){
		
			union{
				short i_16;
				char i_8[2];
			};
			
			// Converte o 8 bits em 16. (altamente inseguro devido os diferentes endiands entre máquinas
			i_8[0] = resp.buff[0];
			i_8[1] = resp.buff[1];
			
			world_obstacles[i_16].profile = ((gam_obj_pack_type *)(&resp.buff[2]))->profile;  // profile
			world_obstacles[i_16].body.pos.x = ((gam_obj_pack_type *)(&resp.buff[2]))->pos_x; // posição x
			world_obstacles[i_16].body.pos.y = ((gam_obj_pack_type *)(&resp.buff[2]))->pos_y; // posição y
			world_obstacles[i_16].graph = graphs_profiles[world_obstacles[i_16].profile];
		}	
	}
	
	#define RELOAD_RETRY (1.0/30.0) *2.0
	static float retry_time =0.0;
	retry_time -= dt;
	if(retry_time <= 0){
		for(short i =0; i <  MAX_OBSTACLES;++i){
			if(!world_obstacles[i].graph.img){
				
				// Converte o 8 bits em 16. (altamente inseguro devido os diferentes endiands entre máquinas
				
				union{
					short i_16;
					char i_8[2];
				};
				i_16 =i;
				packet_type req = {
					{
						0, // a função de send se preocupa com o pack count
						2, // Só transmitirá 2 bytes de buffer
						OBSTACLE_UPDATE //
					},
					{i_8[0],i_8[1]} // O indice do objeto faltante
				};
				
				sendPacket(req);
				
				#ifdef ON_DEBUG
					char buff [30];
					sprintf(buff,"Request obstacle %d",i);
					debugTrace(buff);
				#endif
				retry_time = RELOAD_RETRY;
				return 0;
			}
		}
		
	}
	else return 0;
	packet_type req{
		{
			0,
			0,
			WAINTING_GAME
		},
		{}
	
	};
	sendPacket(req);	
	
	player1.body.pos.x = PLAYER_INIT_X;
	player1.body.pos.y = PLAYER_INIT_Y;
	
	player2.body.pos.x = PLAYER_INIT_X;
	player2.body.pos.y = PLAYER_INIT_Y;
	
	new_round = true;
	debugTrace("clientGetObstacles: loading finished");
	return 1;
	
}


/**
 *  @brief Acha quais são os objetos de fronteira em relação ao objeto referência
 *  
 *  @param [in] ref   Objeto de referência
 *  @param [out] left  Objeto mais a esquerda
 *  @param [out] right Objeto mais a direita
 *  @return Return_Description
 *  
 *  @details Details
 */
bool setObstaclesRange (const game_object_type &ref,int &left, int &right){
	
	// Enquanto o objeto.pos.x mais a esquerda for menor que o canto mais a esquerda da tela
	while((world_obstacles[left].body.pos.x + world_obstacles[left].graph.w) < ref.body.pos.x - PLAYER_FIX_POS){
		if(++left == total_obstacles)
			left = total_obstacles-1;
			return false;// impede que estoure o buffer
	}
	// Enquanto o objeto mais a direita (+1) for menor que o canto direito da tela
	while(world_obstacles[right+1].body.pos.x < ref.body.pos.x +(SCREEN_W - PLAYER_FIX_POS)){
		if(++right == total_obstacles)
			right = total_obstacles-1;
			return false; // Impede que estoure o buffer
	}
}

/**
 *  @brief Brief
 *  
 *  @param [in] ref Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
bool atualizaObjetos (game_object_type &ref,const int &left_index,const int &right_index){
	

	// Imprime todos aqueles que estão dentro do range da tela
	for (int i = left_index; i <= right_index; ++i){
		float obj_x = world_obstacles[i].body.pos.x -  ref.body.pos.x + PLAYER_FIX_POS;
		print(vetor2d_type{obj_x,world_obstacles[i].body.pos.y}, &world_obstacles[i].graph);
	
	}
	
	float ref_x = (ref.body.pos.x < PLAYER_FIX_POS) ? ref.body.pos.x: PLAYER_FIX_POS;
	print(vetor2d_type{ref_x, ref.body.pos.y},&ref.graph);
}

/**
*	@brief Inicializa as condições para um novo jogo posicionando
*  o player no local certo e gerando um novo mapa.
*	@param dt Nessa função não há utilidade a não ser compatibilidade com os demais states
*
*   @return 1 para passar para o próximo estado, -1 em caso de erro
*/
int loadSingleGame (float dt){

	player1.body.pos.x = PLAYER_INIT_X;
	player1.body.pos.y = PLAYER_INIT_Y;
	
	player1.body.speed.setVector(0,0);
	total_obstacles = 0;

	initObstacles();
	
	

	
	ground_offset = 0;
	left_obstacles_index = 0;
	right_obstacles_index = 0;
	
	// Limpa qualquer tecla que esteja em buffer do teclado.
	if(kbhit())
		getch();
	fflush(stdin);

	return 1; // next state = preLancamento
}

/**
 *  @brief Brief
 *  
 *  @param [in] ref Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int setLaunchVector (game_object_type &ref){

}

/**
*	@brief Estado no qual o jogo aguarda o jogador escolher o ângulo e 
*	o momento em que ele deseja lançar o player1.
*
*	@param dt delta tempo para a atualização do demonstrativo do ângulo escolhido
*	@return 0 caso o jogador ainda não tenha feito a escolha, 1 para quando deve-se iniciar o jogo.
*/
int preLancamento (float dt){
	int key;
	static int forca = 2000;
	static int angulo =0;
	static float fatorF = 0.1; 
	
	fatorF = variaForca(fatorF);
	
	if(kbhit()){
		key = (int)getch();
		
		switch(key){
			
			case SPACE:
				player1.body.speed.setVector(forca*fatorF,angulo);
				
				angulo =0;
				forca = 2000;
				return 1;
			case UP:
				angulo = angulo+5;
				if(angulo >90)
					angulo = 90;
				break;
			case DOWN:
				angulo = angulo-5;
				if(angulo <0)
					angulo = 0;
				break;
				
				
		}
		key = 0;
		
	}
	
	drawProgressBar(fatorF*BAR_MAX_HEIGHT, vetor2d_type {(player1.body.pos.x +player1.graph.w/2)-(BAR_WIDTH/2) ,-80}); // desenha barra de força
	groundStep( &player1, &ground,dt);	
	atualizaObjetos(player1,0,0);
	printDirection(vetor2d_type{player1.body.pos.x+(player1.graph.w/2) ,player1.body.pos.y+(player1.graph.h/2)}, angulo,300);
	return 0; // preLancamento
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int preLancamentoMult (float dt){
	
	packet_type player_pos;
	if(getPacket(player_pos) > 0){
		if(player_pos.ctrl. operation == PLAYER_STATUS){
			player2.body.pos.x = ((gam_obj_pack_type *)(&player_pos.buff[0]))->pos_x; // posição x
			player2.body.pos.y = ((gam_obj_pack_type *)(&player_pos.buff[0]))->pos_y; // posição y
		}
	}
	print(player2.body.pos,&player2.graph);
	return preLancamento(dt);
}

/**
*	@brief Estado no qual o jogo está rodando. 
*
*	@param dt delta tempo para o cálculo do espaço percorrido.
*	@return 0 para o jogo em andamento, 1 para quando a velocidade seja igual a 0.
*/
int singleStep (float dt){
	
	static int left_index = 0;
	static int right_index = 0;
	static int last_colide = -1;
	static int red_aura_frames = 0;
	static int green_aura_frames = 0;
	
	lancamento(&player1,dt);
	floorCheck(&player1);
	groundStep( &player1, &ground, dt);
	
	
	setObstaclesRange(player1,left_index,right_index);
	atualizaObjetos(player1,left_index,right_index);


	if(green_aura_frames){
		--green_aura_frames;
		print(vetor2d_type{PLAYER_FIX_POS - ((green_aura.graph.w - player1.graph.w)/2),player1.body.pos.y - ((green_aura.graph.h - player1.graph.h)/2)},&green_aura.graph);
	}
	
	if(red_aura_frames){
		--red_aura_frames;
		print(vetor2d_type{PLAYER_FIX_POS - ((red_aura.graph.w - player1.graph.w)/2),player1.body.pos.y  - ((red_aura.graph.h - player1.graph.h)/2)},&red_aura.graph);
	}

	
	for(int i = left_index; i <= right_index;++i){
		if(last_colide < i){ // Maior pois o player nunca anda para trás
			if(colide(player1,world_obstacles[i])){
				last_colide = i;
				if(profile_collision_bonus[world_obstacles[i].profile]>0){
					player1.body.speed.x *= 1 + profile_collision_bonus[world_obstacles[i].profile];
					player1.body.speed.y*= 1 + profile_collision_bonus[world_obstacles[i].profile];
					
					green_aura_frames = 30;
					red_aura_frames = 0;
				}
				else if(profile_collision_bonus[world_obstacles[i].profile]<0){
					player1.body.speed.x *= 0.9 + profile_collision_bonus[world_obstacles[i].profile];
					player1.body.speed.y*= 0.9 + profile_collision_bonus[world_obstacles[i].profile];
					
					red_aura_frames = 30;
					green_aura_frames = 0;
				}
				break; // Afinal só colide um por vez
			}
		}
	}

	//limitador de velocidade
	if(player1.body.speed.y> SPEED_LIM_Y)
		player1.body.speed.y =SPEED_LIM_Y;	
	if(player1.body.speed.y< -SPEED_LIM_Y)
		player1.body.speed.y = -SPEED_LIM_Y;
	if(player1.body.speed.x> SPEED_LIM_X)
		player1.body.speed.x =SPEED_LIM_X;	
	if(player1.body.speed.x< -SPEED_LIM_X)
		player1.body.speed.x = -SPEED_LIM_X;

	char  texto [100];
	sprintf(texto,"Distancia:\n %d metros",(int)(player1.body.pos.x - PLAYER_INIT_X)/50);
	setcolor(COLOR(255,255,255));
	fontSize(1);
	printTxt(texto, vetor2d_type{SCREEN_W-(textwidth(texto)+20), 20});
	
	if(player1.body.speed.modulo())
		return 0;     //singleStep

	if(kbhit()){
		getch();
		fflush(stdin);
	}
	
	left_index = 0; 
	right_index = 0;
	last_colide = -1;
	red_aura_frames = 0;
	green_aura_frames = 0;
	total_score+= (int)(player1.body.pos.x - PLAYER_INIT_X)/50;
	total_rounds--;
	return 1;
}

int multiStep (float dt){
		
	// Envia a posição do player1 =====================================
	#define RELOAD_RETRY (1.0/30.0) * 2 // De certa forma dá lag mas evita overflow na rede
	static float retry_time =0.0;
	retry_time -= dt;
	if(retry_time <= 0){
	
		packet_type report = {
			{
				0, // a função de send se preocupa com o pack count
				sizeof(gam_obj_pack_type),
				PLAYER_STATUS //
			},
			{}
		};
		
		gam_obj_pack_type player_pos{
				player1.profile,
				player1.body.pos.x,
				player1.body.pos.y,
				player1.body.speed.modulo()
		};
				
		memcpy(&report.buff[0],&player_pos,sizeof(gam_obj_pack_type));
		sendPacket(report);
		retry_time = RELOAD_RETRY;
	}
	// =================================================================
	packet_type player2_pos;
	if(getPacket(player2_pos) > 0){
		if(player2_pos.ctrl. operation == PLAYER_STATUS){
			player2.body.pos.x = ((gam_obj_pack_type *)(&player2_pos.buff[0]))->pos_x; // posição x
			player2.body.pos.y = ((gam_obj_pack_type *)(&player2_pos.buff[0]))->pos_y; // posição y
		}
	}
	
	if(((player2.body.pos.x + player2.graph.w) > player1.body.pos.x - PLAYER_FIX_POS) &&
	   (player2.body.pos.x < player1.body.pos.x +(SCREEN_W - PLAYER_FIX_POS)))
		print(player2.body.pos,&player2.graph);
	
	return singleStep(dt);
}

/**
 *  @brief Brief
 *  
 *  @param [in] dt Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
int singleEnd(float dt){
	char *texto ="TENTE NOVAMENTE", score[50];
	int ret = 1;
	int left_index = 0;
	static int right_index = 0;
	
	groundStep( &player1, &ground, dt);
	
	setObstaclesRange(player1,left_index,right_index);
	atualizaObjetos(player1,left_index,right_index);
	
	sprintf(score,"Você percorreu\n %d metros em %d rodadas.",(int)total_score,5-total_rounds);
	
	setcolor(COLOR(255,255,255));
	fontSize(2);
	printTxt(score, vetor2d_type{(SCREEN_W/2)-(textwidth(score)/2), SCREEN_H/2-(textheight(score)+20)});
	if(total_rounds<=0){
	
		setcolor(COLOR(255,0,0));
		texto ="GAME OVER";
		ret=2;
	}
	fontSize(5);
	printTxt(texto, vetor2d_type{(SCREEN_W/2)-(textwidth(texto)/2), SCREEN_H/2});

	if(kbhit()){
		
		if(ret == 2){
			total_score = 0;
			total_rounds =5;
		}
		
		left_index = 0;
		right_index = 0;
		

		return ret;
		 
	}
	
	return 0;
}

/**
*	@brief Testa o contato do player com o chão 
*
*	@param player - game_object_type do jogador
*/
void floorCheck(game_object_type *player){
	
	if(player->body.pos.y<=0){
			player->body.pos.y=0;
						
			player->body.speed.y = -BOUNCE*player->body.speed.y;
			atrito(player, ATRITO, TARGET_FRAME_RATE);
			
			if(player->body.speed.modulo() < 17)
				player->body.speed.setVector(0,0);
		}
}

/**
*	@brief Função que calcula o deslocamento do chão em funão da posição do player1 
*
*	@param objeto 	player
*	@param ground   ground
* 	@param dt delta tempo para o cálculo do espaço percorrido.
*/
void groundStep(game_object_type *objeto, game_object_type *ground, float dt){
	

	if(objeto->body.pos.x>= PLAYER_FIX_POS)
		ground_offset = ground_offset - objeto->body.speed.x*dt;
		
	if(ground_offset <=(float)-ground->graph.w)
		ground_offset = ground_offset + ground->graph.w;
	
	for(int i =0; i<=(SCREEN_W/ground->graph.w)+1;i++){
		int posground=i*ground->graph.w+ ground_offset;
		print(vetor2d_type{posground, ground->body.pos.y}, &ground->graph);
	}	
}

float variaForca(float valor){
	static float incremento = 0.02;

		valor  = valor + incremento;
		if(valor>1){
			valor = 1;
			incremento = -incremento;
		}
		if(valor<0.1){
			valor = 0.1;
			incremento = -incremento;
		}	

	return valor;	
	
}

void resetGame(){
	

	total_obstacles = 0;
	total_score = 0; 
	total_rounds = 5;	
	left_obstacles_index = 0;
	right_obstacles_index = 0;
	player1.body.pos.x = PLAYER_INIT_X;
	player1.body.pos.y = PLAYER_INIT_Y;	
	player1.body.speed.setVector(0,0);
	ground_offset = 0;
	resetLoja();	

	//inicializa o fator inicial de bonus
	for(int i = 0; i < NUM_BLOCKS;++i)	profile_collision_bonus[i] = 0.0;

}

void resetLoja(){
	//Itens da loja
	int coluna, linha, j=0;
	for(int i = 0; i < NUM_LOJA_MENU; ++i){
		loja_options[i].graph = graphs_profiles[LOJA_OPTION_1 + i];
		
		if (i == NUM_LOJA_MENU-2){
			coluna = SCREEN_W -graphs_profiles[LOJA_OPTION_1 + i].w -20;
			linha = (500)-(graphs_profiles[LOJA_OPTION_1 + i].h +10);}
		else if (i == NUM_LOJA_MENU-1){
			coluna = SCREEN_W -graphs_profiles[LOJA_OPTION_1 + i].w -20;
			linha = (500)-(graphs_profiles[LOJA_OPTION_1 + i].h*2 +10);}
		else if (i%2 == 0){
			coluna = 20;
			linha = (500)-(graphs_profiles[LOJA_OPTION_1 + i].h +10)*j;
			j++;
		} else
			coluna = graphs_profiles[LOJA_OPTION_1 + i].w +25;
			
		
		vetor2d_type loja_pos{coluna,linha};
		loja_options[i].body.pos =	loja_pos;
	}
}
