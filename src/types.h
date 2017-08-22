#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define MAP_WIDTH 24
#define MAP_HEIGHT 31

#define SPRITE_NUM 2
#define WALL_TEXT_NUM 7

#define PROJECT_X(x, y, dirx, diry, screenx, screeny) (diry*x - dirx*y)/(screenx*diry - screeny*dirx)
#define PROJECT_Y(x, y, dirx, diry, screenx, screeny) (screenx*y - screeny*x)/(screenx*diry - screeny*dirx)

#define SCREEN_DIST 0.9
#define PERPENDICULAR_X(x, y) SCREEN_DIST*y
#define PERPENDICULAR_Y(x, y) -SCREEN_DIST*x

int THREAD_NUM;

typedef struct{
    double x, y, z, angle;
    int text;
    double screenx, screeny;
    double dist;
} Sprite;

typedef struct{
    double x, y;
    double dirx, diry;
    double speed;
    double screenx, screeny;
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
    int side;
} Ray;

typedef struct{
    int height;
    int drawStart, drawEnd;
    int textx, texty;
    double wallx;
} Vertdraw;

typedef struct{
    int width, height;
    int drawStart, drawEnd;
} Horizodraw;

typedef struct{
    double x, y;
    int textx, texty;
    int ctextx, ctexty;
    double wallx, wally;
    double dist, w;
} Floordraw;
