#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include<math.h>
#include "../h/types.h"

#define GRAVIDADE -10


// Protótipos ======================================================
void lancamento(game_object_type *p, float dt);
bool colide (game_object_type &c, game_object_type &p);
void atrito (game_object_type *a, float coefatrito, float dt);
// =================================================================



#endif // __PHYSICS_H__
