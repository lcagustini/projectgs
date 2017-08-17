#ifndef SDL

#define SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#endif

#ifndef TYPES

#define TYPES
#include "types.h"

#endif

void initGfx(SDL_Surface **textures, SDL_Surface **sprites, SDL_Surface *screen);
Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Window *getWindow();
void exitWindow(SDL_Window *window);
SDL_Surface *loadTexture(char *path, SDL_Surface *screen);
