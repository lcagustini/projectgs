#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define mapWidth 24
#define mapHeight 24

#define RED 0xFF0000
#define BLUE 0x0000FF
#define GREEN 0x00FF00
#define PURPLE 0xAA00FF
#define YELLOW 0x00FFFF

int worldMap[mapWidth][mapHeight]=
{
    {RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,BLUE,BLUE,BLUE,BLUE,BLUE,0,0,0,0,GREEN,0,GREEN,0,GREEN,0,0,0,RED},
    {RED,0,0,0,0,0,BLUE,0,0,0,BLUE,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,BLUE,0,0,0,BLUE,0,0,0,0,GREEN,0,0,0,GREEN,0,0,0,RED},
    {RED,0,0,0,0,0,BLUE,0,0,0,BLUE,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,BLUE,BLUE,0,BLUE,BLUE,0,0,0,0,GREEN,0,GREEN,0,GREEN,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,YELLOW,0,YELLOW,0,0,0,0,YELLOW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,YELLOW,0,0,0,0,PURPLE,0,YELLOW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,YELLOW,0,YELLOW,0,0,0,0,YELLOW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,YELLOW,0,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,YELLOW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,0,0,0,0,0,0,0,0,0,0,0,0,0,0,RED},
    {RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED,RED}
};

typedef struct{
    double x, y;
    double dirx, diry;
    double speed;
} Player;

typedef struct{
    double x, y;
    double dirx, diry;
    double angle;
    int gridx, gridy;
    double distx, disty;
    double deltax, deltay;
    double screenDist;
    int stepx, stepy;
} Ray;

typedef struct{
    double x, y;
} Screen;

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
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return NULL;
    }
    return window;
}

void loop(SDL_Window *window){
    SDL_Surface *screenSurface = NULL;
    SDL_Event e;
    int side, height, drawStart, drawEnd;
    Player player;
    Ray ray;
    Screen screen;

    player.x = player.y = 12;
    player.dirx = 0;
    player.diry = 1;
    player.speed = 0.5;

    screen.x = 0.66;
    screen.y = 0;

    screenSurface = SDL_GetWindowSurface(window);
    Uint64 curTime = SDL_GetPerformanceCounter();
    Uint64 lastTime = 0;
    double dt = 0;
    double aux;
    while(true){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                return;
            }
            else if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        if(!worldMap[(int) (player.x + player.dirx * player.speed)][(int) player.y]) player.x += player.dirx * player.speed;
                        if(!worldMap[(int) player.x][(int) (player.y + player.diry * player.speed)]) player.y += player.diry * player.speed;
                        break;

                    case SDLK_DOWN:
                        if(!worldMap[(int) (player.x - player.dirx * player.speed)][(int) player.y]) player.x -= player.dirx * player.speed;
                        if(!worldMap[(int) player.x][(int) (player.y - player.diry * player.speed)]) player.y -= player.diry * player.speed;
                        break;

                    case SDLK_LEFT:
                        aux = player.dirx;
                        player.dirx = player.dirx * cos(-player.speed/3) - player.diry * sin(-player.speed/3);
                        player.diry = aux * sin(-player.speed/3) + player.diry * cos(-player.speed/3);

                        aux = screen.x;
                        screen.x = screen.x * cos(-player.speed/3) - screen.y * sin(-player.speed/3);
                        screen.y = aux * sin(-player.speed/3) + screen.y * cos(-player.speed/3);
                        break;

                    case SDLK_RIGHT:
                        aux = player.dirx;
                        player.dirx = player.dirx * cos(player.speed/3) - player.diry * sin(player.speed/3);
                        player.diry = aux * sin(player.speed/3) + player.diry * cos(player.speed/3);

                        aux = screen.x;
                        screen.x = screen.x * cos(player.speed/3) - screen.y * sin(player.speed/3);
                        screen.y = aux * sin(player.speed/3) + screen.y * cos(player.speed/3);
                        break;

                    default:
                        printf("A");
                        break;
                }
            }
        }
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x000000, 0x000000, 0x000000));
        for(int i = 0; i < SCREEN_WIDTH; i++){
            ray.angle = 2*i/SCREEN_WIDTH -1;
            ray.x = player.x;
            ray.y = player.y;
            ray.dirx = player.dirx + screen.x*ray.angle;
            ray.diry = player.diry + screen.y*ray.angle;

            ray.gridx = (int) player.x;
            ray.gridy = (int) player.y;

            ray.deltax = sqrt(1 + (ray.diry * ray.diry) / (ray.dirx * ray.dirx));
            ray.deltay = sqrt(1 + (ray.dirx * ray.dirx) / (ray.diry * ray.diry));

            if(ray.dirx < 0){
                ray.stepx = -1;
                ray.distx = (ray.x - ray.gridx) * ray.deltax;
            }
            else{
                ray.stepx = 1;
                ray.distx = (ray.gridx - ray.x +1) * ray.deltax;
            }
            if(ray.diry < 0){
                ray.stepy = -1;
                ray.disty = (ray.y - ray.gridy) * ray.deltay;
            }
            else{
                ray.stepy = 1;
                ray.disty = (ray.gridy - ray.y +1) * ray.deltay;
            }

            while(true){
                if(ray.distx < ray.disty){
                    ray.distx += ray.deltax;
                    ray.gridx += ray.stepx;
                    side = 0;
                }
                else{
                    ray.disty += ray.deltay;
                    ray.gridy += ray.stepy;
                    side = 1;
                }
                if(worldMap[ray.gridx][ray.gridy])
                    break;
            }

            if(!side)
                ray.screenDist = (ray.gridx - ray.x + (1 - ray.stepx) / 2) / ray.dirx;
            else
                ray.screenDist = (ray.gridy - ray.y + (1 - ray.stepy) / 2) / ray.diry;

            height = (int) SCREEN_HEIGHT/ray.screenDist;

            drawStart = SCREEN_HEIGHT/2 - height/2;
            drawEnd = drawStart + height;
            if(drawStart < 0) drawStart = 0;
            if(drawEnd > SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT;

            if(SDL_MUSTLOCK(screenSurface))
                SDL_LockSurface(screenSurface);

            for(int j = drawStart; j < drawEnd; j++){
                if(side = 1)
                    putPixel(screenSurface, i, j, worldMap[ray.gridx][ray.gridy]);
                else
                    putPixel(screenSurface, i, j, worldMap[ray.gridx][ray.gridy]/2);
            }

            if(SDL_MUSTLOCK(screenSurface))
                SDL_UnlockSurface(screenSurface);
        }

        lastTime = curTime;
        curTime = SDL_GetPerformanceCounter();
        dt = (double) ((abs(curTime - lastTime)) / (double) SDL_GetPerformanceFrequency());
        //printf("FPS: %lf\n", 1/dt);
        printf("x: %lf - y: %lf\n", player.x, player.y);
        SDL_UpdateWindowSurface(window);
    }
}

void exitWindow(SDL_Window *window){
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]){
    SDL_Window *window = getWindow();
    loop(window);
    exitWindow(window);
    return 0;
}
