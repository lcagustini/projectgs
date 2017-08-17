#ifndef SDL

#define SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#endif

#ifndef TYPES

#define TYPES
#include "types.h"

#endif

void handleKeys(int worldMap[][MAP_HEIGHT], Player *p, double dt, SDL_Event e);
