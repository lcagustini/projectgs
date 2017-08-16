#ifndef SDL

#define SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#endif

#ifndef TYPES

#define TYPES
#include "types.h"

#endif

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Window *getWindow();
SDL_Surface *loadSprite(char *path, SDL_Surface *screen);
SDL_Surface *loadTexture(char *path, SDL_Surface *screen);
