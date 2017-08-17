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

#define PROJECT_X(x, y, dirx, diry, screenx, screeny) (diry*x - dirx*y)/(screenx*diry - screeny*dirx)
#define PROJECT_Y(x, y, dirx, diry, screenx, screeny) (screenx*y - screeny*x)/(screenx*diry - screeny*dirx)

int worldMap[MAP_WIDTH][MAP_HEIGHT]=
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

void spriteSort(Sprite *sprites, int start, int end){
   int i, j;
   Sprite pivot, t;

   if(start < end){
       pivot = sprites[start];
       i = start;
       j = end+1;

       while(1){
           do ++i; while(sprites[i].dist >= pivot.dist && i <= end);
           do --j; while(sprites[j].dist < pivot.dist);
           if(i >= j) break;

           t = sprites[i];
           sprites[i] = sprites[j];
           sprites[j] = t;
       }
       t = sprites[start];
       sprites[start] = sprites[j];
       sprites[j] = t;

       spriteSort(sprites, start, j-1);
       spriteSort(sprites, j+1, end);
   }
}

void loop(SDL_Window *window){
    SDL_Event e;
    double depth[SCREEN_WIDTH];
    double aux;

    Player player;

    SDL_Surface *screenSurface = NULL;
    SDL_Surface *textures[5];

    SDL_Surface *spriteText[SPRITE_NUM];
    Sprite sprites[SPRITE_NUM] = {{.x = 20, .y = 12, .text = 0}, {.x = 12, .y = 20, .text = 0}};

    Vertdraw spriteVert;
    Horizodraw spriteHorizo;
    Uint32 color;

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

        SDL_FillRect(screenSurface, &botScreen, SDL_MapRGB(screenSurface->format, 0xb2, 0xb2, 0xb2));
        SDL_FillRect(screenSurface, &topScreen, SDL_MapRGB(screenSurface->format, 0x60, 0x60, 0x60));

        Ray ray;
        for(int i = 0; i < SCREEN_WIDTH; i++){
            initRay(&ray, i, player);

            rayDDA(worldMap, &ray);
            drawCastedWall(worldMap, &ray, i, textures, screenSurface);

            depth[i] = ray.screenDist;
        }

        for(int k = 0; k < SPRITE_NUM; k++){
            sprites[k].screenx = sprites[k].x - player.x;
            sprites[k].screeny = sprites[k].y - player.y;
            sprites[k].dist = sprites[k].screenx*sprites[k].screenx + sprites[k].screeny*sprites[k].screeny;
        }
        spriteSort(sprites, 0, SPRITE_NUM -1);

        for(int k = 0; k < SPRITE_NUM; k++){
            aux = sprites[k].screenx;
            sprites[k].screenx = PROJECT_X(aux, sprites[k].screeny, player.dirx, player.diry, player.screenx, player.screeny);
            sprites[k].screeny = PROJECT_Y(aux, sprites[k].screeny, player.dirx, player.diry, player.screenx, player.screeny);

            sprites[k].screenx = (int) ((SCREEN_WIDTH/2) * (sprites[k].screenx/sprites[k].screeny +1));
            spriteVert.height = abs((int) (SCREEN_HEIGHT/sprites[k].screeny));

            spriteVert.drawStart = SCREEN_HEIGHT/2 - spriteVert.height/2;
            spriteVert.drawEnd = spriteVert.drawStart + spriteVert.height;
            if(spriteVert.drawStart < 0) spriteVert.drawStart = 0;
            if(spriteVert.drawEnd > SCREEN_HEIGHT) spriteVert.drawEnd = SCREEN_HEIGHT;

            spriteHorizo.width = spriteVert.height;

            spriteHorizo.drawStart = sprites[k].screenx - spriteHorizo.width/2;
            spriteHorizo.drawEnd = spriteHorizo.drawStart + spriteHorizo.width;
            if(spriteHorizo.drawStart < 0) spriteHorizo.drawStart = 0;
            if(spriteHorizo.drawEnd > SCREEN_WIDTH) spriteHorizo.drawEnd = SCREEN_WIDTH;

            for(int i = spriteHorizo.drawStart; i < spriteHorizo.drawEnd; i++){
                spriteVert.textx = (int) (256 * (i - (sprites[k].screenx - spriteHorizo.width/2)) * spriteText[sprites[k].text]->w/spriteHorizo.width) /256;
                if(sprites[k].screeny > 0 && sprites[k].screeny < depth[i]){
                    for(int j = spriteVert.drawStart; j < spriteVert.drawEnd; j++){
                        spriteVert.texty = (((j*256 - SCREEN_HEIGHT*128 + spriteVert.height*128) * spriteText[sprites[k].text]->h) / spriteVert.height) /256;
                        color = getPixel(spriteText[sprites[k].text], spriteVert.textx, spriteVert.texty);
                        if(color != 0xFF00FF)
                            putPixel(screenSurface, i, j, color);
                    }
                }
            }
        }

        lastTime = curTime;
        curTime = SDL_GetPerformanceCounter();
        dt = (double) ((abs(curTime - lastTime)) / (double) SDL_GetPerformanceFrequency());
        //printf("FPS: %lf\n", 1/dt);
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
