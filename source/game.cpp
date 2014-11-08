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
int loadSingleGame (float dt);
int preLancamento (float dt);
int singleStep (float dt);
void floorCheck(game_object_type *player);
void groundStep(game_object_type *objeto, game_object_type *ground, float dt);
int singleEnd(float dt);


//Variáveis privadas ============================================
game_object_type player1,player2, ground;
game_object_type world_obstacles[MAX_OBSTACLES];
graph_data_type graphs_profiles[NUM_OBJECTS_DEFINE];
unsigned int left_obstacles_index = 0,right_obstacles_index = 0;
int  ground_offset;
int total_score = 0; 
int total_rounds = 5;

game_state_type load_state ={
	initGame,
	(game_state_type*[]){
		&load_state,		 // return 0
		&load_menu_state	 // return 1
	}
};

game_state_type load_menu_state ={
	initMenu,
	(game_state_type*[]){
		&load_menu_state, // return 0
		&menu_state       // return 1
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
		&pre_lancamento  // return 1
	},
};

game_state_type pre_lancamento ={
	preLancamento,
	(game_state_type*[]){
		&pre_lancamento,	// return 0
		&step_single 		// return 1
	},
};
game_state_type end_single ={
	singleEnd,
	(game_state_type*[]){
		&end_single,	 // return 0
		&load_single,	 // return 1
		&load_menu_state // return 2
	},
};
game_state_type step_single ={
	singleStep,
	(game_state_type*[]){
		&step_single,	 // return 0
		&end_single     // return 1
	},
};

game_state_type *game_states =  &load_state;

/**
 *  @brief Inicia os recursos que serão utilzados no game
 *  
 *  @param dt
 */ 
int initGame (float dt){
	
	// FAZER LOAD DO ARQUIVO COM OS NOMES DAS IMAGENS ==========
	using namespace std;	
	ifstream file(RESOURCES_ROOT);
	
	char data[3000];
	int pos =0;
	while(file.good()){
		data[pos] = file.get();
		pos++;
	}
	data[pos] = '\0';
	
	char * pch;
		
	pch = strtok(data,",");//carrega o primeiro booleano
	
	for(int i = 0; i <NUM_OBJECTS_DEFINE; ++i){
		static char temp[200],tempmsk[200], t_width[4], t_height[5];
		std::cout<<" ciclo = "<<i<<std::endl;

		
		//carrega valor da largura
		strcpy(t_width,pch);
		graphs_profiles[i].w = atoi(t_width);
		std::cout<<" width = "<<graphs_profiles[i].w<<std::endl;

		//carrega valor da altura
		pch = strtok(NULL,",");
		strcpy(t_height,pch);
		graphs_profiles[i].h = atoi(t_height);
		std::cout<<" height = "<<graphs_profiles[i].h<<std::endl;
		
		//carrega caminho da mascara

		pch = strtok(NULL,",");
		strcpy(tempmsk,pch);	
		std::cout<<" tempmsk = "<<tempmsk<<std::endl;

		pch = strtok(NULL,"\n\0");
		strcpy(temp,pch);
		std::cout<<" tempo = "<<temp<<std::endl;
		pch = strtok(NULL,",");
		
		
		graphInitObjects(&graphs_profiles[i], temp,tempmsk);
	}
	
	player1.graph = graphs_profiles[PLAYER1];
	player2.graph = graphs_profiles[PLAYER2];
	ground.graph = graphs_profiles[GROUND];
	ground.body.pos.x = 0;
	ground.body.pos.y = -10 ;
	ground.body.speed.x = 0;
	ground.body.speed.y = 0;
	
	return 1;
}

/**
 *  @brief Limpa a memória heap
 */
void endGame (void){
	
	for(int i = 0; i <NUM_OBJECTS_DEFINE; ++i){
		delete graphs_profiles[i].img;
	}
}


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
		player1.body.speed.y *= -1;
	}
	// Verifica se atingiu o chão
	if(player2.body.pos.y<=0){
		player2.body.pos.y=0;
		player2.body.speed.y *= -1;
	}
	
	
	print(player1.body.pos,&player1.graph, OR_PUT);
	print(player2.body.pos,&player2.graph, OR_PUT);
	// -------------------------------------------

	if(kbhit()) return 1;
	
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

	for(int i = 0; i < MAX_OBSTACLES;++i){

		// Randomiza qual o perfil (tipo) de obstáculo ele será, Igreja, nuvem....
		unsigned int obj_profile = (rand() % NUM_BLOCKS);
		world_obstacles[i].collision_mask = MASK_BIT(obj_profile);
		world_obstacles[i].graph = graphs_profiles[obj_profile]; // Aponta para qual o bitmap que pertence aquele perfil
		
		//partindo da posição minima para deixar fora da tela, a posição de cada obstaculo varia 100px,
		//a partir do primeiro objeto colocado a posicao minima vira a posição do objeto anterior
		//e a ele é somado a largura desse mesmo objeto objeto anterior, de forma a evitar a sobreposição
		world_obstacles[i].body.pos.x = ((rand() % 600) + 50) + ((i != 0) ? world_obstacles[i-1].graph.w + world_obstacles[i-1].body.pos.x: SCREEN_W - 50);
		
		if(obj_profile == NUVEM_POLUICAO)
			world_obstacles[i].body.pos.y = ((rand() % 300) + 50);
		else
			world_obstacles[i].body.pos.y = 0;

	}
}

void loadMultiGame (void){

}


void connectToServer (void){

}


/**
 *  @brief Brief
 *  
 *  @param [in] ref Parameter_Description
 *  @return Return_Description
 *  
 *  @details Details
 */
bool atualizaObjetos (game_object_type &ref){
	
	float ref_x = (ref.body.pos.x < PLAYER_FIX_POS) ? ref.body.pos.x: PLAYER_FIX_POS;
	
	// Enquanto o objeto.pos.x mais a esquerda for menor que o canto mais a esquerda da tela
	while((world_obstacles[left_obstacles_index].body.pos.x + world_obstacles[left_obstacles_index].graph.w) < ref.body.pos.x - PLAYER_FIX_POS){
		if(++left_obstacles_index == MAX_OBSTACLES)
			return false;// impede que estoure o buffer
	}
	// Enquanto o objeto mais a direita (+1) for menor que o canto direito da tela
	while(world_obstacles[right_obstacles_index+1].body.pos.x < ref.body.pos.x +(SCREEN_W - PLAYER_FIX_POS)){
		if(++right_obstacles_index == MAX_OBSTACLES)
			return false; // Impede que estoure o buffer
	}
	
	// Imprime todos aqueles que estão dentro do range da tela
	for (int i = left_obstacles_index; i <= right_obstacles_index; ++i){
		float obj_x = world_obstacles[i].body.pos.x -  ref.body.pos.x + PLAYER_FIX_POS;
		print(vetor2d_type{obj_x,world_obstacles[i].body.pos.y}, &world_obstacles[i].graph, OR_PUT);
	
	}
	print(vetor2d_type{ref_x, ref.body.pos.y},&ref.graph, OR_PUT);
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
	

	initObstacles();
	
	ground_offset = 0;
	left_obstacles_index = 0;
	right_obstacles_index = 0;
	
	// Limpa qualquer tecla que esteja em buffer do teclado.
	getch();
	fflush(stdin);

	return 1; // next state = preLancamento
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
	
	if(kbhit()){
		key = (int)getch();
		
		switch(key){
			
			case SPACE:
				player1.body.speed.setVector(forca,angulo);
				
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
	
	
	groundStep( &player1, &ground,dt);	
	atualizaObjetos(player1);
	printDirection(vetor2d_type{player1.body.pos.x+(player1.graph.w/2) ,player1.body.pos.y+(player1.graph.h/2)}, angulo,300);

	return 0; // preLancamento
}
/**
*	@brief Estado no qual o jogo está rodando. 
*
*	@param dt delta tempo para o cálculo do espaço percorrido.
*	@return 0 para o jogo em andamento, 1 para quando a velocidade seja igual a 0.
*/
int singleStep (float dt){
	
	lancamento(&player1,dt);
	floorCheck(&player1);
	groundStep( &player1, &ground, dt);
	atualizaObjetos(player1);
	
	
	

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

	total_score+= (int)(player1.body.pos.x - PLAYER_INIT_X)/50;
	total_rounds--;
	return 1;
}

int singleEnd(float dt){
	char *texto ="TENTE NOVAMENTE", score[50];
	int ret = 1;
	groundStep( &player1, &ground, dt);
	atualizaObjetos(player1);
	
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
						
			player->body.speed.y = -0.5*player->body.speed.y;
			atrito(player, 5, 0.1);
			
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
