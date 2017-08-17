#include "input.h"

void handleKeys(int worldMap[][MAP_HEIGHT], Player *p, double dt){
    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W]){
        if(!worldMap[(int) (p->x + p->dirx * p->speed *dt)][(int) p->y]) p->x += p->dirx * p->speed *dt;
        if(!worldMap[(int) p->x][(int) (p->y + p->diry * p->speed *dt)]) p->y += p->diry * p->speed *dt;
    }
    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]){
        if(!worldMap[(int) (p->x - p->dirx * p->speed *dt)][(int) p->y]) p->x -= p->dirx * p->speed *dt;
        if(!worldMap[(int) p->x][(int) (p->y - p->diry * p->speed *dt)]) p->y -= p->diry * p->speed *dt;
    }
    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]){
        if(!worldMap[(int) (p->x + p->dirx * p->speed *dt)][(int) p->y]) p->x += p->diry * p->speed *dt;
        if(!worldMap[(int) p->x][(int) (p->y + p->diry * p->speed *dt)]) p->y -= p->dirx * p->speed *dt;
    }
    if(SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]){
        if(!worldMap[(int) (p->x - p->dirx * p->speed *dt)][(int) p->y]) p->x -= p->diry * p->speed *dt;
        if(!worldMap[(int) p->x][(int) (p->y - p->diry * p->speed *dt)]) p->y += p->dirx * p->speed *dt;
    }
}

void handleMouse(Player *p, double dt, SDL_Event e){
    double aux;
    double mouse = e.motion.xrel;

    aux = p->dirx;
    p->dirx = p->dirx * cos(mouse *dt) - p->diry * sin(mouse *dt);
    p->diry = aux * sin(mouse *dt) + p->diry * cos(mouse *dt);

    aux = p->screenx;
    p->screenx = p->screenx * cos(mouse *dt) - p->screeny * sin(mouse *dt);
    p->screeny = aux * sin(mouse *dt) + p->screeny * cos(mouse *dt);
}
