#include "sprite.h"

void quickSort(Sprite *sprites, int start, int end){
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

       quickSort(sprites, start, j-1);
       quickSort(sprites, j+1, end);
   }
}

void spriteSort(Sprite *sprites, Player player){
    for(int k = 0; k < SPRITE_NUM; k++){
        sprites[k].screenx = sprites[k].x - player.x;
        sprites[k].screeny = sprites[k].y - player.y;
        sprites[k].dist = sprites[k].screenx*sprites[k].screenx + sprites[k].screeny*sprites[k].screeny;
    }
    quickSort(sprites, 0, SPRITE_NUM -1);
}

void initSprite(Player player, int k, Sprite *sprites, Horizodraw *spriteHorizo, Vertdraw *spriteVert){
    double aux;

    aux = sprites[k].screenx;
    sprites[k].screenx = PROJECT_X(aux, sprites[k].screeny, player.dirx, player.diry, player.screenx, player.screeny);
    sprites[k].screeny = PROJECT_Y(aux, sprites[k].screeny, player.dirx, player.diry, player.screenx, player.screeny);

    sprites[k].screenx = (int) ((SCREEN_WIDTH/2) * (sprites[k].screenx/sprites[k].screeny +1));
    spriteVert->height = abs((int) (SCREEN_HEIGHT/sprites[k].screeny));

    spriteVert->drawStart = SCREEN_HEIGHT/2 - spriteVert->height/2;
    spriteVert->drawEnd = spriteVert->drawStart + spriteVert->height;
    if(spriteVert->drawStart < 0) spriteVert->drawStart = 0;
    if(spriteVert->drawEnd > SCREEN_HEIGHT) spriteVert->drawEnd = SCREEN_HEIGHT;

    spriteHorizo->width = spriteVert->height;
    spriteHorizo->height = (int) (sprites[k].z/sprites[k].screeny);

    spriteHorizo->drawStart = sprites[k].screenx - spriteHorizo->width/2;
    spriteHorizo->drawEnd = spriteHorizo->drawStart + spriteHorizo->width;
    if(spriteHorizo->drawStart < 0) spriteHorizo->drawStart = 0;
    if(spriteHorizo->drawEnd > SCREEN_WIDTH) spriteHorizo->drawEnd = SCREEN_WIDTH;
}

int getIndex(double x, double y){
    int index;

    index = (int) (2*((0.95*atan2(y, x)) +3)/3);

    return index;
}

void drawSprites(Player player, Sprite *sprites, SDL_Surface *spriteText[][4], SDL_Surface *screenSurface, double *depth){
    Vertdraw spriteVert;
    Horizodraw spriteHorizo;
    Uint32 color;
    int index;

    for(int k = 0; k < SPRITE_NUM; k++){
        index = getIndex(sprites[k].screenx, sprites[k].screeny);
        initSprite(player, k, sprites, &spriteHorizo, &spriteVert);
        for(int i = spriteHorizo.drawStart; i < spriteHorizo.drawEnd; i++){
            spriteVert.textx = (int) (256 * (i - (sprites[k].screenx - spriteHorizo.width/2)) * spriteText[sprites[k].text][index]->w/spriteHorizo.width) /256;
            if(sprites[k].screeny > 0 && sprites[k].screeny < depth[i]){
                for(int j = spriteVert.drawStart; j < spriteVert.drawEnd; j++){
                    spriteVert.texty = ((((j - spriteHorizo.height)*256 - SCREEN_HEIGHT*128 + spriteVert.height*128) * spriteText[sprites[k].text][index]->h) / spriteVert.height) /256;
                    color = getPixel(spriteText[sprites[k].text][index], spriteVert.textx, spriteVert.texty);
                    if(color != 0xFF00FF)
                        putPixel(screenSurface, i, j, color);
                }
            }
        }
    }
}
