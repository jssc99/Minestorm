#pragma once
#include "physics.h"

typedef struct Player
{
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    Vector2 inertia;
    Sat sat[3];
    float size;
    float speed;
    int lives;
    bool displayAxis;
    bool displaySpeed;
    bool displayInertia;
    bool displaySS;
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
void draw_player(Player* p, unsigned int color);
Player rotate_player(Player p, float angle);
Player update_player(Player p, float deltaTime);
Player turnleft_player(Player p, float deltaTime);
Player turnright_player(Player p, float deltaTime);
Player accelerate_player(Player p, float deltaTime);
void SS_collision_border_replace(Point2 *p, float size);
void on_off(bool *b);
void debug_menu_player(Player* p, bool debugPlayer);

// Bullet
Bullet init_bullet(Player p);
Bullet update_bullet(Bullet b,  float deltaTime);
void draw_bullet(Point2 center, unsigned int sides, float radius, unsigned int color);

