#include "gfx.h"

void initGfx(SDL_Surface **textures, SDL_Surface **sprites, SDL_Surface *screen){
    sprites[0] = loadTexture("gfx/blastoise.png", screen);

    textures[0] = loadTexture("gfx/brick.png", screen);
    textures[1] = loadTexture("gfx/wood.png", screen);
    textures[2] = loadTexture("gfx/stone.png", screen);
    textures[3] = loadTexture("gfx/sand.png", screen);
    textures[4] = loadTexture("gfx/pumpkin.png", screen);
}

Uint32 getPixel(SDL_Surface *surface, int x, int y){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[(y*surface->w) + x];
}

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y*surface->w) + x] = pixel;
}

SDL_Window *getWindow(){
    SDL_Window *window = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    window = SDL_CreateWindow("projectgs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return NULL;
    }
    return window;
}

void exitWindow(SDL_Window *window){
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Surface *loadTexture(char *path, SDL_Surface *screen){
    SDL_Surface *load = NULL, *opt = NULL;

    load = IMG_Load(path);
    if(load == NULL){
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return NULL;
    }
    opt = SDL_ConvertSurface(load, screen->format, 0);
    SDL_FreeSurface(load);
    if(opt == NULL){
        printf("Unable to optimize image! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }

    return opt;
}
