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

#define ATRITO 50
#define BOUNCE 0.5
#define SPEED_LIM_X 2000
#define SPEED_LIM_Y 2500

// typedefs privados ===========================================
typedef struct{
	char *file_name;
	int heigth;
	int width;
}init_file_type;


// Prot�tipos Privados ==========================================
int initGame (float dt);
int initMenu (float dt);
int showMenu (float dt);
int loadSingleGame (float dt);
int preLancamento (float dt);
int singleStep (float dt);
void floorCheck(game_object_type *player);
void groundStep(game_object_type *objeto, game_object_type *ground, float dt);
int singleEnd(float dt);
int showCredits (float dt);
float variaForca(float valor);

//Vari�veis privadas ============================================
game_object_type player1,player2, ground;
game_object_type green_aura, red_aura;
game_object_type menu_options[NUM_OPTIONS_MENU];
game_object_type world_obstacles[MAX_OBSTACLES];
graph_data_type graphs_profiles[NUM_OBJECTS_DEFINE];
unsigned int left_obstacles_index = 0,right_obstacles_index = 0;
float  ground_offset;
float obstacles_weight[NUM_BLOCKS],max_obstacles_per_type[NUM_BLOCKS]; 
int total_obstacles = 0;
int total_score = 0; 
int total_rounds = 5;
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
		&termina_programa,		 // return 2	
		&termina_programa,		 // return 3
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
game_state_type end_single ={
	singleEnd,
	(game_state_type*[]){
		&end_single,	 // return 0
		&load_single,	 // return 1
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
game_state_type *game_states =  &load_state;

static void debugTrace (char *msg){
#ifdef ON_DEBUG
	std::cout << "debugTrace: " << msg << std::endl;
#endif	
};

/**
 *  @brief Inicia os recursos que ser�o utilzados no game
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
		
		// Carrega e captura a imagem na mem�ria
		graphInitObjects(&graphs_profiles[i], temp,tempmsk);
	}
	// Inicializa o gr�fico do player1
	player1.graph = graphs_profiles[PLAYER1];
#ifdef ON_DEBUG
	player1.collision_mask |=0xFFFFFFFF;// MASK_BIT(CONGRESSO) | MASK_BIT(LGBT) | MASK_BIT(BANCO);
#endif
	// Inicializa o gr�fico do player2
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
	
	// Feedback visuais de colis�o
	green_aura.graph = graphs_profiles[GREEN_AURA];
	red_aura.graph = graphs_profiles[RED_AURA];
	
	//carrega lista de pesos dos objetos
	for(int i = 0; i < NUM_BLOCKS; i++) obstacles_weight[i] = 1;
	
	
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
	
	printTxt("Anderson Ara�jo", vetor2d_type{(SCREEN_W/2)-(textwidth("Anderson Ara�jo")/2), SCREEN_H/2-(textheight("Anderson Ara�jo"))});
	printTxt("Carol Fernandes", vetor2d_type{(SCREEN_W/2)-(textwidth("Carol Fernandes")/2), SCREEN_H/2-(textheight("Carol Fernandes")-20)});
	printTxt("Diego Ortiz", vetor2d_type{(SCREEN_W/2)-(textwidth("Diego Ortiz")/2), SCREEN_H/2-(textheight("Diego Ortiz")-40)});
	printTxt("Lucas Pina", vetor2d_type{(SCREEN_W/2)-(textwidth("Lucas Pina")/2), SCREEN_H/2-(textheight("Lucas Pina")-60)});
	printTxt("Marcelo Pietragala", vetor2d_type{(SCREEN_W/2)-(textwidth("Marcelo Pietragala")/2), SCREEN_H/2-(textheight("Marcelo Pietragala")-80)});
	
	if(kbhit())
		return -1;
		
	return 0;
}

/**
 *  @brief Limpa a mem�ria heap
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
	// Pura gra�a -------------------------------
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
	
	// Pura gra�a -------------------------------
	lancamento(&player1,dt);
	lancamento(&player2,dt);
	// Verifica se atingiu o ch�o
	if(player1.body.pos.y<=0){
		player1.body.pos.y=0;
		player1.body.speed.y *= -0.99;
	}
	// Verifica se atingiu o ch�o
	if(player2.body.pos.y<=0){
		player2.body.pos.y=0;
		player2.body.speed.y *= -0.99;
	}
	print(vetor2d_type{0,-10},&graphs_profiles[LOGOTIPO], COPY_PUT);
	print(player1.body.pos,&player1.graph, OR_PUT);
	print(player2.body.pos,&player2.graph, OR_PUT);
	// -------------------------------------------
	
	
	for(int i = 0; i < NUM_OPTIONS_MENU; ++i)
		print(menu_options[i].body.pos,&menu_options[i].graph);

	for(int i = 0; i < NUM_OPTIONS_MENU; ++i){
		if(GetKeyState(VK_LBUTTON)&0x80){ //clique do mouse
			POINT mouse_pos;
			if (GetCursorPos(&mouse_pos)){ //retorna uma estrutura que cont�m a posi��o atual do cursor
				char debug[50];
								
				HWND hwnd = GetForegroundWindow(); 
				if (ScreenToClient(hwnd, &mouse_pos)){
					
					// Passa o y para as coordenadas de nossos objetos
					mouse_pos.y = SCREEN_H - mouse_pos.y;
					// Verifica os limites de x
					if((mouse_pos.x < (int)menu_options[i].bottomLeft().x) || (mouse_pos.x > (int)(menu_options[i].topRight().x))) continue;
					// Verifica os limites de y
					if((mouse_pos.y < (int)menu_options[i].bottomLeft().y) || (mouse_pos.y > (int)menu_options[i].topRight().y)) continue;
					// Retorna a op��o do menu			
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
 *  @return Return_Description
 *  
 *  @details Details
 */
void initObstacles (void){
	//carrega lista de n�meros m�ximos de cada objeto
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

		// Randomiza qual o perfil (tipo) de obst�culo ele ser�, Igreja, nuvem....
		unsigned int obj_profile = (rand() % NUM_BLOCKS);
		
		//checa se o profile selecionado j� alcan�ou o n�mero limite previsto 
		//se sim, sorteia novamente
		//se n�o, segue com o procedimento de carregar ele na lista
		if(max_obstacles_per_type[obj_profile]>0){
		
				world_obstacles[obstacles_defined].collision_mask = MASK_BIT(obj_profile);
				world_obstacles[obstacles_defined].graph = graphs_profiles[obj_profile]; // Aponta para qual o bitmap que pertence aquele perfil
				
				//partindo da posi��o minima para deixar fora da tela, a posi��o de cada obstaculo varia 100px,
				//a partir do primeiro objeto colocado a posicao minima vira a posi��o do objeto anterior
				//e a ele � somado a largura desse mesmo objeto objeto anterior, de forma a evitar a sobreposi��o
				world_obstacles[obstacles_defined].body.pos.x = ((rand() % 600) + 50) + ((obstacles_defined != 0) ? world_obstacles[obstacles_defined-1].graph.w + world_obstacles[obstacles_defined-1].body.pos.x: SCREEN_W - 50);
				
				if(obj_profile == NUVEM_POLUICAO)
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
 *  @return Return_Description
 *  
 *  @details Details
 */
void loadMultiGame (void){

}

/**
 *  @brief Brief
 *  
 *  @return Return_Description
 *  
 *  @details Details
 */
void connectToServer (void){

}

/**
 *  @brief Acha quais s�o os objetos de fronteira em rela��o ao objeto refer�ncia
 *  
 *  @param [in] ref   Objeto de refer�ncia
 *  @param [out] left  Objeto mais a esquerda
 *  @param [out] right Objeto mais a direita
 *  @return Return_Description
 *  
 *  @details Details
 */
bool setObstaclesRange (const game_object_type &ref,int &left, int &right){
	
	// Enquanto o objeto.pos.x mais a esquerda for menor que o canto mais a esquerda da tela
	while((world_obstacles[left].body.pos.x + world_obstacles[left].graph.w) < ref.body.pos.x - PLAYER_FIX_POS){
		if(++left == total_obstacles){
			
			left = total_obstacles-1;
			return false;// impede que estoure o buffer
		}
	}
	// Enquanto o objeto mais a direita (+1) for menor que o canto direito da tela
	while(world_obstacles[right+1].body.pos.x < ref.body.pos.x +(SCREEN_W - PLAYER_FIX_POS)){
		if(++right == total_obstacles){
		
			right = total_obstacles-1;
			return false; // Impede que estoure o buffer
		}
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
	

	// Imprime todos aqueles que est�o dentro do range da tela
	for (int i = left_index; i <= right_index; ++i){
		float obj_x = world_obstacles[i].body.pos.x -  ref.body.pos.x + PLAYER_FIX_POS;
		print(vetor2d_type{obj_x,world_obstacles[i].body.pos.y}, &world_obstacles[i].graph,OR_PUT);
	
	}
	
	float ref_x = (ref.body.pos.x < PLAYER_FIX_POS) ? ref.body.pos.x: PLAYER_FIX_POS;
	print(vetor2d_type{ref_x, ref.body.pos.y},&ref.graph, OR_PUT);
}

/**
*	@brief Inicializa as condi��es para um novo jogo posicionando
*  o player no local certo e gerando um novo mapa.
*	@param dt Nessa fun��o n�o h� utilidade a n�o ser compatibilidade com os demais states
*
*   @return 1 para passar para o pr�ximo estado, -1 em caso de erro
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
*	@brief Estado no qual o jogo aguarda o jogador escolher o �ngulo e 
*	o momento em que ele deseja lan�ar o player1.
*
*	@param dt delta tempo para a atualiza��o do demonstrativo do �ngulo escolhido
*	@return 0 caso o jogador ainda n�o tenha feito a escolha, 1 para quando deve-se iniciar o jogo.
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
	
	drawProgressBar(fatorF*BAR_MAX_HEIGHT, vetor2d_type {(player1.body.pos.x +player1.graph.w/2)-(BAR_WIDTH/2) ,-80}); // desenha barra de for�a
	groundStep( &player1, &ground,dt);	
	atualizaObjetos(player1,0,0);
	printDirection(vetor2d_type{player1.body.pos.x+(player1.graph.w/2) ,player1.body.pos.y+(player1.graph.h/2)}, angulo,300);

	return 0; // preLancamento
}

/**
*	@brief Estado no qual o jogo est� rodando. 
*
*	@param dt delta tempo para o c�lculo do espa�o percorrido.
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
		print(vetor2d_type{PLAYER_FIX_POS - ((green_aura.graph.w - player1.graph.w)/2),player1.body.pos.y - ((green_aura.graph.h - player1.graph.h)/2)},&green_aura.graph,OR_PUT);
	}
	
	if(red_aura_frames){
		--red_aura_frames;
		print(vetor2d_type{PLAYER_FIX_POS - ((red_aura.graph.w - player1.graph.w)/2),player1.body.pos.y  - ((red_aura.graph.h - player1.graph.h)/2)},&red_aura.graph,OR_PUT);
	}

	
	for(int i = left_index; i <= right_index;++i){
		if(last_colide != i){
			if(colide(player1,world_obstacles[i])){
				last_colide = i;
				if(player1.collision_mask & world_obstacles[i].collision_mask){
					player1.body.speed.x *= 2.0;
					player1.body.speed.y*= 2.0;
					
					green_aura_frames = 30;
					red_aura_frames = 0;
				}
				else{
					player1.body.speed.x *=0.50;
					player1.body.speed.y*= 0.50;
					
					red_aura_frames = 30;
					green_aura_frames = 0;
				}
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

	total_score+= (int)(player1.body.pos.x - PLAYER_INIT_X)/50;
	total_rounds--;
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
int singleEnd(float dt){
	char *texto ="TENTE NOVAMENTE", score[50];
	int ret = 1;
	int left_index = 0;
	static int right_index = 0;
	
	groundStep( &player1, &ground, dt);
	
	setObstaclesRange(player1,left_index,right_index);
	atualizaObjetos(player1,left_index,right_index);
	
	sprintf(score,"Voc� percorreu\n %d metros em %d rodadas.",(int)total_score,5-total_rounds);
	
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
*	@brief Testa o contato do player com o ch�o 
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
*	@brief Fun��o que calcula o deslocamento do ch�o em fun�o da posi��o do player1 
*
*	@param objeto 	player
*	@param ground   ground
* 	@param dt delta tempo para o c�lculo do espa�o percorrido.
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
