#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16

#define mapWidth 24
#define mapHeight 24

typedef struct{
    double x, y;
    int text;
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
