#ifndef SDL

#define SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#endif

#ifndef TYPES

#define TYPES
#include "types.h"

#endif

#include <stdio.h>

#include "ray.h"
#include "gfx.h"
#include "input.h"

#define SCREEN_DIST 0.666666
#define PERPENDICULAR_X(x, y) SCREEN_DIST*y
#define PERPENDICULAR_Y(x, y) -SCREEN_DIST*x

int worldMap[mapWidth][mapHeight]=
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void initPlayer(Player *p){
    p->x = p->y = 15;
    p->dirx = -1;
    p->diry = 0;
    p->speed = 200;

    p->screenx = PERPENDICULAR_X(p->dirx, p->diry);
    p->screeny = PERPENDICULAR_Y(p->dirx, p->diry);
}

void loop(SDL_Window *window){
    SDL_Event e;

    double depth[SCREEN_WIDTH];

    Player player;

    SDL_Surface *screenSurface = NULL;
    SDL_Surface *textures[5];

    SDL_Surface *spriteText[1];
    Sprite blastoise;

    screenSurface = SDL_GetWindowSurface(window);

    initPlayer(&player);
    initGfx(textures, spriteText, screenSurface);

    SDL_Rect topScreen = {.x = 0, .y = 0, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT/2 -1};
    SDL_Rect botScreen = {.x = 0, .y = SCREEN_HEIGHT/2, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT -1};

    Uint64 curTime = SDL_GetPerformanceCounter();
    Uint64 lastTime = 0;
    double dt = 0;
    while(1){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                return;
            }
            else if(e.type == SDL_KEYDOWN){
                handleKeys(worldMap, &player, dt, e);
            }
        }

        SDL_FillRect(screenSurface, &botScreen, SDL_MapRGB(screenSurface->format, 0xb20000, 0x00b200, 0x0000b2));
        SDL_FillRect(screenSurface, &topScreen, SDL_MapRGB(screenSurface->format, 0x600000, 0x006000, 0x000060));

        Ray ray;
        for(int i = 0; i < SCREEN_WIDTH; i++){
            initRay(&ray, i, player);

            rayDDA(worldMap, &ray);
            drawCastedWall(worldMap, &ray, i, textures, screenSurface);

            depth[i] = ray.screenDist;
        }

        SDL_BlitSurface(spriteText[0], NULL, screenSurface, NULL);

        lastTime = curTime;
        curTime = SDL_GetPerformanceCounter();
        dt = (double) ((abs(curTime - lastTime)) / (double) SDL_GetPerformanceFrequency());
        printf("FPS: %lf\n", 1/dt);
        //printf("x: %lf - y: %lf\n", player.x, player.y);
        SDL_UpdateWindowSurface(window);
    }
}

int main(int argc, char* args[]){
    SDL_Window *window = getWindow();
    loop(window);
    exitWindow(window);
    return 0;
}
