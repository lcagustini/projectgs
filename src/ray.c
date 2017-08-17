#include "ray.h"
#include "gfx.h"

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

void rayDDA(int worldMap[][MAP_HEIGHT], Ray *ray){
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

void drawCastedFloor(Ray *ray, int id, SDL_Surface **textures, SDL_Surface *screenSurface, Player *player, Vertdraw *strip){
    Floordraw floor;

    if(!ray->side && ray->dirx > 0){
        floor.wallx = ray->gridx;
        floor.wally = ray->gridy + strip->wallx;
    }
    else if(!ray->side && ray->dirx < 0){
        floor.wallx = ray->gridx + 1;
        floor.wally = ray->gridy + strip->wallx;
    }
    else if(ray->side && ray->diry > 0){
        floor.wallx = ray->gridx + strip->wallx;
        floor.wally = ray->gridy;
    }
    else{
        floor.wallx = ray->gridx + strip->wallx;
        floor.wally = ray->gridy + 1;
    }

    for(int j = strip->drawEnd +1; j < SCREEN_HEIGHT; j++){
        floor.dist = SCREEN_HEIGHT / (2.0*j - SCREEN_HEIGHT);

        floor.w = floor.dist/ray->screenDist;

        floor.x = floor.w*floor.wallx + (1 - floor.w)*player->x;
        floor.y = floor.w*floor.wally + (1 - floor.w)*player->y;

        floor.textx = (int) (floor.x * textures[0]->w) % textures[0]->w;
        floor.texty = (int) (floor.y * textures[0]->h) % textures[0]->h;

        floor.ctextx = (int) (floor.x * textures[1]->w) % textures[1]->w;
        floor.ctexty = (int) (floor.y * textures[1]->h) % textures[1]->h;

        putPixel(screenSurface, id, j, getPixel(textures[0], floor.textx, floor.texty));
        putPixel(screenSurface, id, SCREEN_HEIGHT - j, getPixel(textures[1], floor.ctextx, floor.ctexty));
    }
}

void drawCastedWall(int worldMap[][MAP_HEIGHT], Ray *ray, int id, SDL_Surface **textures, SDL_Surface *screenSurface, Vertdraw *strip){
    SDL_Surface *texture;

    //Projects the casted ray position to the screen vector
    if(!ray->side)
        ray->screenDist = (ray->gridx - ray->x + (1 - ray->stepx) / 2) / ray->dirx;
    else
        ray->screenDist = (ray->gridy - ray->y + (1 - ray->stepy) / 2) / ray->diry;

    //Gets the height of the casted wall and calculate drawing points
    strip->height = (int) SCREEN_HEIGHT/ray->screenDist;

    strip->drawStart = SCREEN_HEIGHT/2 - strip->height/2;
    strip->drawEnd = strip->drawStart + strip->height;
    if(strip->drawStart < 0) strip->drawStart = 0;
    if(strip->drawEnd > SCREEN_HEIGHT) strip->drawEnd = SCREEN_HEIGHT;

    //Calculates where in the wall tile the ray hit
    if(!ray->side)
        strip->wallx = ray->y + ray->screenDist*ray->diry;
    else
        strip->wallx = ray->x + ray->screenDist*ray->dirx;
    strip->wallx -= (int) strip->wallx;

    //Get current texture
    texture = textures[worldMap[ray->gridx][ray->gridy] -1];

    //Calculates which x of the wall texture should be rendered
    strip->textx = (int) (strip->wallx * texture->w);
    if((!ray->side && ray->dirx > 0) || (ray->side && ray->diry < 0))
        strip->textx = texture->w - strip->textx -1;

    if(SDL_MUSTLOCK(screenSurface))
        SDL_LockSurface(screenSurface);

    //Goes through the texture vertically and draws to the screen surface
    for(int j = strip->drawStart; j < strip->drawEnd; j++){
        strip->texty = (((j*256 - SCREEN_HEIGHT*128 + strip->height*128) * texture->h) / strip->height) /256;
        if(ray->side)
            putPixel(screenSurface, id, j, getPixel(textures[worldMap[ray->gridx][ray->gridy] -1], strip->textx, strip->texty));
        else
            putPixel(screenSurface, id, j, DARKER(getPixel(textures[worldMap[ray->gridx][ray->gridy] -1], strip->textx, strip->texty), 2));
    }

    if(SDL_MUSTLOCK(screenSurface))
        SDL_UnlockSurface(screenSurface);
}

void drawCastedStrip(int worldMap[][MAP_HEIGHT], Ray *ray, int id, SDL_Surface **textures, SDL_Surface *screenSurface, Player *player){
    Vertdraw strip;

    drawCastedWall(worldMap, ray, id, textures, screenSurface, &strip);
    drawCastedFloor(ray, id, textures, screenSurface, player, &strip);
}
