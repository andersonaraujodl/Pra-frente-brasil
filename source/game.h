#ifndef __GAME_H__
#define __GAME_H__

#include "h/types.h"


// Typedefs p�blicos ====================================
typedef struct game_state_type{

	int (*func)(float dt);
	game_state_type **states;
	int num_states;
}game_state_type;

// Prot�tipos P�blicos ==================================
void initGame (void);


// Vari�veis p�blicas ===================================
extern game_state_type *game_states;
extern game_state_type menu_state;
extern game_state_type pre_lancamento;
extern game_state_type step_single;
extern game_state_type load_single;

#endif