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

#define DARKER(a, b) (((a & 0xFF0000)/b) & 0xFF0000) + (((a & 0x00FF00)/b) & 0x00FF00) + (((a & 0x0000FF)/b) & 0x0000FF)

void initRay(Ray *ray, int id, Player player);
void rayDDA(int worldMap[][MAP_HEIGHT], Ray *ray);
void drawCastedStrip(int worldMap[][MAP_HEIGHT], Ray *ray, int id, SDL_Surface **textures, SDL_Surface *screenSurfacel, Player *player);
