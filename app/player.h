#pragma once
#include "physics.h"

typedef struct Player
{
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    Vector2 inertia;
    Point2 shape[3];
    Sat sat[3];
    float size;
    float speed;
    float firecd;
    int lives;
    bool displayAxis;
    bool displaySpeed;
    bool displayInertia;
    bool displaySSphere;
    bool collisionTests;
} Player;

typedef struct Bullet
{
    Point2 location;
    Vector2 direction;
    float size;
    float lifespan;
} Bullet;

//Draw

void draw_circle(Point2 *cBox, Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color);

// Player
Player player_init(Player p, float x, float y, float size);
void draw_player(Player* p, unsigned int color);
void rotate_player(Player* p, float angle);
void update_player(Player* p, float deltaTime, Point2 maxScreen);
void  turnleft_player(Player* p, float deltaTime);
void  turnright_player(Player* p, float deltaTime);
Player accelerate_player(Player p, float deltaTime);
void on_off(bool *b);
void debug_menu_player(Player* p, bool debugPlayer);


// Bullet
Bullet init_bullet(Player p);
void update_bullet(Bullet* b,  float deltaTime, Point2 maxScreen);
void draw_bullet(Point2 center, unsigned int sides, float radius, unsigned int color);

//Test collisions
void test_collision(Player player1, ImVec2 mousePos);
