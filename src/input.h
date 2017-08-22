#ifndef SDL

#define SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif

#ifndef TYPES

#define TYPES
#include "types.h"

#endif

#define COLLISION_CIRCLE 10

void handleMovement(int worldMap[][MAP_HEIGHT], Player *p, double dt);
void handleKeys(Player *p, SDL_Event e);
void handleMouse(Player *p, double dt, SDL_Event e);
