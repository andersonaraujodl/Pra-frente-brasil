#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include<math.h>

#define GRAVIDADE 10


// Protótipos ======================================================
void lancamento(game_object_type *p, float dt);
void colisao (game_object_type *c, game_object_type *p);
// =================================================================



#endif // __PHYSICS_H__
