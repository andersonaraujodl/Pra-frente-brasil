#ifndef __GAME_H__
#define __GAME_H__

#include "h/types.h"


// Typedefs p�blicos ====================================
typedef struct game_state_type{

	int (*func)(float dt);
	game_state_type **states;
	int num_states;
}game_state_type;


// defines p�blicos =====================================
#define UP 72
#define DOWN 80
#define SPACE (int)' '
#define ESC 27
#define TARGET_FRAME_RATE (1.0/60.0)
// Prot�tipos P�blicos ==================================
void keyCapture();


// Vari�veis p�blicas ===================================
extern game_state_type *game_states;
extern game_state_type menu_state;
extern game_state_type pre_lancamento;
extern game_state_type step_single;
extern game_state_type load_single;

#endif
