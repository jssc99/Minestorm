#pragma once
#include "Tools/axis.h"
#include <stdbool.h>

typedef struct Player
{
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    Vector2 inertia;
    float size;
    float speed;
    int lives;
} Player;

typedef struct Bullet
{
    Point2 location;
    Vector2 direction;
    float size;
    float lifespan;
} Bullet;

// Player
Player player_init(Player p, float x, float y, float size);
void draw_player(Player p, unsigned int color, float sz);
Player rotate_player(Player p, float angle);
Player update_player(Player p);
Player turnleft_player(Player p);
Player turnright_player(Player p);
Player accelerate_player(Player p);
void SS_collision_border_replace(Point2 *p, float size);

// Bullet
Bullet init_bullet(Player p);
Bullet update_bullet(Bullet b);
void draw_bullet(Point2 center, unsigned int sides, float radius, unsigned int color);

// Physics
bool SS_collision_rectangle(Point2 p, float size, float xmin, float ymin, float xmax, float ymax);