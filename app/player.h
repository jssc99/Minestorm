#include "Tools/axis.h"

typedef struct Player
{
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    float speed;
    int lives;
} Player;

void player_init(Player p, int x, int y);
void draw_player(Player p, unsigned int color, float sz);
