#include "player.h"

// Initialise Player at position(x,y) First time think about setting lives to 3.
int player_init(Player p, int x, int y)
{
    p.axis = (Axis2){(x, y), (1, 0), (0, 1)};
    p.moveLine = (Vector2){1, 1};
    p.targetLine = p.moveLine;
    p.speed = 0;
}

// draws the player
int draw_player(Player p)
{
    
    return 0;
}