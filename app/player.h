#pragma once
#include "physics.h"
//#include "draw.c"
#define MAX_BULLETS 5
static const float MAX_SPEED_SHIP = 500;
static const float ACCELERATION = 20;
static const float DECELERATION = 0.7;

typedef struct Bullet
{
    Point2 location;
    Vector2 direction;
    float size;
    float lifespan;
} Bullet;

typedef struct Player
{
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    Vector2 inertia;
    Point2 shape[10];
    float size;
    float speed;
    float firecd;
    int lives;
    bool displayAxis;
    bool displaySpeed;
    bool displayInertia;
    bool displaySSphere;
    bool collisionTests;
    Bullet bullets[MAX_BULLETS]; // Rename
} Player;

//Draw

void draw_circle(Point2 *cBox, Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color);

// Player
Player player_init( float x, float y, float size);
void init_points_player(Player *p);
void player_spawn(Player *p, float x, float y);
void draw_player(Player p, int playerNb);
//void draw_player(Player* p, unsigned int color);
void draw_debug_player(Player *p);
void rotate_player(Player* p, float angle);
void update_player(Player* p, float deltaTime, Point2 maxScreen, bool p2);
void  turnleft_player(Player* p, float deltaTime);
void  turnright_player(Player* p, float deltaTime);
Player accelerate_player(Player p, float deltaTime);
void on_off(bool *b);
void debug_menu_player(Player* p, bool debugPlayer);


// Bullet
Bullet init_bullet(Player p);
void update_one_bullet(Bullet* b,  float deltaTime, Point2 maxScreen);
void update_bullet(Player *p, float deltaTime, Point2 maxScreen);
void draw_bullet(Point2 center, float radius, unsigned int color);
void fire_bullet(Player *p, float deltaTime, Point2 maxScreen);

//Test collisions
void test_collision(Player player1, ImVec2 mousePos);
