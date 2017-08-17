#include "input.h"

void handleKeys(int worldMap[][MAP_HEIGHT], Player *p, double dt, SDL_Event e){
    double aux;

    switch(e.key.keysym.sym){
        case SDLK_UP:
            if(!worldMap[(int) (p->x + p->dirx * p->speed *dt)][(int) p->y]) p->x += p->dirx * p->speed *dt;
            if(!worldMap[(int) p->x][(int) (p->y + p->diry * p->speed *dt)]) p->y += p->diry * p->speed *dt;
            break;

        case SDLK_DOWN:
            if(!worldMap[(int) (p->x - p->dirx * p->speed *dt)][(int) p->y]) p->x -= p->dirx * p->speed *dt;
            if(!worldMap[(int) p->x][(int) (p->y - p->diry * p->speed *dt)]) p->y -= p->diry * p->speed *dt;
            break;

        case SDLK_RIGHT:
            aux = p->dirx;
            p->dirx = p->dirx * cos(-p->speed *dt/3) - p->diry * sin(-p->speed *dt/3);
            p->diry = aux * sin(-p->speed *dt/3) + p->diry * cos(-p->speed *dt/3);

            aux = p->screenx;
            p->screenx = p->screenx * cos(-p->speed *dt/3) - p->screeny * sin(-p->speed *dt/3);
            p->screeny = aux * sin(-p->speed *dt/3) + p->screeny * cos(-p->speed *dt/3);
            break;

        case SDLK_LEFT:
            aux = p->dirx;
            p->dirx = p->dirx * cos(p->speed *dt/3) - p->diry * sin(p->speed *dt/3);
            p->diry = aux * sin(p->speed *dt/3) + p->diry * cos(p->speed *dt/3);

            aux = p->screenx;
            p->screenx = p->screenx * cos(p->speed *dt/3) - p->screeny * sin(p->speed *dt/3);
            p->screeny = aux * sin(p->speed *dt/3) + p->screeny * cos(p->speed *dt/3);
            break;

        default:
            printf("Q\n");
            break;
    }
}
