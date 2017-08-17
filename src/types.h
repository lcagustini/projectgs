#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

#define SPRITE_NUM 2
#define WALL_TEXT_NUM 7

typedef struct{
    double x, y, z;
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
