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

#include <pthread.h>
#include <stdio.h>

#include "ray.h"
#include "gfx.h"
#include "input.h"
#include "sprite.h"

int worldMap[MAP_WIDTH][MAP_HEIGHT]=
{
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,7,7,7,7,7,7,7,7,0,0,0,0,3,0,3,0,3,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,7,7,0,0,0,0,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,7,7,0,0,0,0,7,7,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,7,7,0,0,0,0,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,7,7,7,0,0,7,7,7,0,0,0,0,3,0,3,0,3,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,7,7,7,0,0,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6}
};

void initPlayer(Player *p){
    p->x = p->y = 15;
    p->dirx = -1;
    p->diry = 0;
    p->speed = 3;

    p->screenx = PERPENDICULAR_X(p->dirx, p->diry, SCREEN_DIST);
    p->screeny = PERPENDICULAR_Y(p->dirx, p->diry, SCREEN_DIST);
}

typedef struct{
    int id;
    Player *player;
    SDL_Surface **textures;
    SDL_Surface *screenSurface;
    double *depth;
} threadArg;

void *drawWorld(void *args){
    Ray ray;
    threadArg *aux = (threadArg*) args;

    for(int i = SCREEN_WIDTH/THREAD_NUM * aux->id; i < ((aux->id == THREAD_NUM-1) ? SCREEN_WIDTH : (SCREEN_WIDTH/THREAD_NUM * (aux->id +1))); i++){
        initRay(&ray, i, *(aux->player));

        rayDDA(worldMap, &ray);
        drawCastedStrip(worldMap, &ray, i, aux->textures, aux->screenSurface, aux->player);

        aux->depth[i] = ray.screenDist;
    }

    return NULL;
}

void loop(SDL_Window *window){
    SDL_Event e;
    double depth[SCREEN_WIDTH];

    pthread_t threadIds[THREAD_NUM];
    threadArg args[THREAD_NUM];

    Player player;

    SDL_Surface *screenSurface = NULL;
    SDL_Surface *textures[WALL_TEXT_NUM];

    SDL_Surface *spriteText[SPRITE_NUM][4];
    Sprite sprites[SPRITE_NUM] = {{.x = 20, .y = 12, .z = 0, .angle = 1, .text = 0}, {.x = 12, .y = 20, .z = 0, .angle = 0, .text = 1}};

    screenSurface = SDL_GetWindowSurface(window);

    initPlayer(&player);
    initGfx(textures, spriteText, screenSurface);

    Uint64 curTime = SDL_GetPerformanceCounter();
    Uint64 lastTime = 0;
    double dt = 0;
    double averageFPS = 0;

    for(int i = 0; i < THREAD_NUM; i++){
        args[i].id = i;
        args[i].player = &player;
        args[i].textures = textures;
        args[i].screenSurface = screenSurface;
        args[i].depth = depth;
    }

    //SDL_SetRelativeMouseMode(SDL_TRUE);
    while(1){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                printf("Average FPS: %lf\n", averageFPS);
                return;
            }
            else if(e.type == SDL_MOUSEMOTION)
                handleMouse(&player, dt, e);
            else if(e.type == SDL_KEYDOWN)
                handleKeys(&player, e);
        }
        handleMovement(worldMap, &player, dt);

        //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
        for(int i = 0; i < THREAD_NUM; i++){
            pthread_create(&threadIds[i], NULL, drawWorld, &args[i]);
        }
        for(int i = 0; i < THREAD_NUM; i++){
            pthread_join(threadIds[i], NULL);
        }

        spriteSort(sprites, player);
        drawSprites(player, sprites, spriteText, screenSurface, depth);

        lastTime = curTime;
        curTime = SDL_GetPerformanceCounter();
        dt = (double) ((abs(curTime - lastTime)) / (double) SDL_GetPerformanceFrequency());
        printf("FPS: %lf\n", 1/dt);

        RENDERED_FRAMES++;
        averageFPS = (averageFPS*RENDERED_FRAMES + 1/dt) / (RENDERED_FRAMES +1);

        SDL_UpdateWindowSurface(window);
    }
}

int main(int argc, char* args[]){
    THREAD_NUM = 2*SDL_GetCPUCount();
    SCREEN_DIST = 0.95;
    RENDERED_FRAMES = 0;

    SDL_Window *window = getWindow();
    loop(window);
    exitWindow(window);
    return 0;
}
