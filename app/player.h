#pragma once
#include "Tools/axis.h"
#include <stdbool.h>


typedef struct Player
{
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    float speed;
    Vector2 inertia;
    float size;
    int lives;
} Player;

Player player_init(Player p, float x, float y, float size);
void draw_player(Player p, unsigned int color, float sz);
Player rotate_player(Player p, float angle);
Player update_player(Player p);
Player turnleft_player(Player p);
Player turnright_player(Player p);
Player accelerate_player(Player p);
