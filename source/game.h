#ifndef __GAME_H__
#define __GAME_H__

#include "h/types.h"


// Typedefs públicos ====================================
typedef struct game_state_type{

	int (*func)(float dt);
	game_state_type **states;
}game_state_type;


// defines públicos =====================================
#define UP 		72
#define DOWN 	80
#define LEFT   	75
#define RIGHT  	77
#define SPACE 	(int)' '
#define PWUP_1	'A'
#define PWUP_2	'S'
#define ESC 27
#define TARGET_FRAME_RATE (1.0/60.0)
// Protótipos Públicos ==================================
void endGame (void);

// Variáveis públicas ===================================
extern game_state_type *game_states;
extern game_state_type load_menu_state;
extern game_state_type menu_state;
extern game_state_type pre_lancamento;
extern game_state_type step_single;
extern game_state_type load_single;

#endif
