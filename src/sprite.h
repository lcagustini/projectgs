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

#include "gfx.h"

void spriteSort(Sprite *sprites, Player player);
void initSprite(Player player, int k, Sprite *sprites, Horizodraw *spriteHorizo, Vertdraw *spriteVert);
void drawSprites(Player player, Sprite *sprites, SDL_Surface *spriteText[][4], SDL_Surface *screenSurface, double *depth);
