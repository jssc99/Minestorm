#include "Tools/axis.h"

typedef struct Player
{
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    float speed;
    int lives;
}Player;

Player player_init(Player p, float x, float y);
void draw_player(Player p, unsigned int color, float sz);
Player rotate_player(Player p, float angle);