#include "ray.h"

void initRay(Ray *ray, int id, Player player){
    ray->angle = 2*id/(double) (SCREEN_WIDTH) -1;
    ray->x = player.x;
    ray->y = player.y;
    ray->dirx = player.dirx + player.screenx*ray->angle;
    ray->diry = player.diry + player.screeny*ray->angle;

    ray->gridx = (int) player.x;
    ray->gridy = (int) player.y;

    ray->deltax = sqrt(1 + (ray->diry * ray->diry) / (ray->dirx * ray->dirx));
    ray->deltay = sqrt(1 + (ray->dirx * ray->dirx) / (ray->diry * ray->diry));

    if(ray->dirx < 0){
        ray->stepx = -1;
        ray->distx = (ray->x - ray->gridx) * ray->deltax;
    }
    else{
        ray->stepx = 1;
        ray->distx = (ray->gridx - ray->x +1) * ray->deltax;
    }
    if(ray->diry < 0){
        ray->stepy = -1;
        ray->disty = (ray->y - ray->gridy) * ray->deltay;
    }
    else{
        ray->stepy = 1;
        ray->disty = (ray->gridy - ray->y +1) * ray->deltay;
    }
}

void rayDDA(int worldMap[][mapHeight], Ray *ray){
    while(1){
        if(ray->distx < ray->disty){
            ray->distx += ray->deltax;
            ray->gridx += ray->stepx;
            ray->side = 0;
        }
        else{
            ray->disty += ray->deltay;
            ray->gridy += ray->stepy;
            ray->side = 1;
        }
        if(worldMap[ray->gridx][ray->gridy])
            break;
    }
}

void drawCastedWall(int worldMap[][mapHeight], Ray *ray, int id, SDL_Surface **textures, SDL_Surface *screenSurface){
    Vertdraw strip;

    //Projects the casted ray position to the screen vector
    if(!ray->side)
        ray->screenDist = (ray->gridx - ray->x + (1 - ray->stepx) / 2) / ray->dirx;
    else
        ray->screenDist = (ray->gridy - ray->y + (1 - ray->stepy) / 2) / ray->diry;

    //Gets the height of the casted wall and calculate drawing points
    strip.height = (int) SCREEN_HEIGHT/ray->screenDist;

    strip.drawStart = SCREEN_HEIGHT/2 - strip.height/2;
    strip.drawEnd = strip.drawStart + strip.height;
    if(strip.drawStart < 0) strip.drawStart = 0;
    if(strip.drawEnd > SCREEN_HEIGHT) strip.drawEnd = SCREEN_HEIGHT;

    //Calculates where in the wall tile the ray hit
    if(!ray->side)
        strip.wallx = ray->y + ray->screenDist*ray->diry;
    else
        strip.wallx = ray->x + ray->screenDist*ray->dirx;
    strip.wallx -= (int) strip.wallx;

    //Calculates which x of the wall texture should be rendered
    strip.textx = (int) (strip.wallx * TEXTURE_WIDTH);
    if((!ray->side && ray->dirx > 0) || (ray->side && ray->diry < 0))
        strip.textx = TEXTURE_WIDTH - strip.textx -1;

    if(SDL_MUSTLOCK(screenSurface))
        SDL_LockSurface(screenSurface);

    //Goes through the texture vertically and draws to the screen surface
    for(int j = strip.drawStart; j < strip.drawEnd; j++){
        strip.texty = (((j*256 - SCREEN_HEIGHT*128 + strip.height*128) * TEXTURE_HEIGHT) / strip.height) /256;
        if(ray->side)
            putPixel(screenSurface, id, j, getPixel(textures[worldMap[ray->gridx][ray->gridy] -1], strip.textx, strip.texty));
        else
            putPixel(screenSurface, id, j, DARKER(getPixel(textures[worldMap[ray->gridx][ray->gridy] -1], strip.textx, strip.texty), 2));
    }


    if(SDL_MUSTLOCK(screenSurface))
        SDL_UnlockSurface(screenSurface);
}
