#pragma once
#include "physics.h"
#include "enemy.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    Bullet bullets[MAX_BULLETS];
    Axis2 axis;
    Point2 spawnPoint;
    Vector2 targetLine;
    Vector2 moveLine;
    Vector2 inertia;
    Point2 shape[10];
    float size;
    float speed;
    float firecd;
    float tpcd;
    int lives;
    bool hideAxis;
    bool hideSpeed;
    bool hideInertia;
    bool hideSSphere;
    bool displayCollisionBox;    
} Player;

// Draw
void draw_player(Player p, int playerNb);
void draw_debug_player(Player *p);
void draw_bullet(Point2 center, float radius, unsigned int color);
void draw_players_bullets(Player *p);

// Player
Player player_init(float x, float y, float size);
void player_spawn(Player *p, float x, float y);
void player_spawn_check(Player *p, Point2 newLocation, Point2 maxScreen, Enemy *e);
void init_points_player(Player *p);
void update_player(Player *p, float deltaTime, Point2 maxScreen, bool p2, Enemy* e);
void rotate_player(Player *p, float angle);

//Inputs
void turnleft_player(Player *p, float deltaTime);
void turnright_player(Player *p, float deltaTime);
void accelerate_player(Player *p, float deltaTime);
void fire_bullet(Player *p, Point2 maxScreen);
void teleport_player(Player *p, Point2 maxScreen, Enemy* e);
void debug_menu_player(Player *p, char playerNumber);

//Bullets
Bullet init_bullet(Player p, Point2 maxScreen);
void update_one_bullet(Bullet *b, float deltaTime, Point2 maxScreen);
void update_bullet(Player *p, float deltaTime, Point2 maxScreen);
void bullets_terminate(Player *p1, Player *p2);

// Animations
void PathLineTo_point2(Point2 p);
void draw_thruster(Point2 p1, Point2 p2, Point2 p3);
void anim_left_thruster(Player p);
void anim_right_thruster(Player p);
void anim_booster(Player p);
void anim_explode(Point2 origin, float maxsize, float deltaTime);

// Test collisions
void test_collision(Player player1, Player player2, ImVec2 mousePos, Enemy e);
bool player_collision_floating(Player *p, Enemy *e);
bool player_collision_mf_mine(Player *p, Enemy *e);
bool player_collision_square_mine(Player *p, Enemy *e);
bool player_collision_minelayer(Player *p, Enemy *e);
bool player_collision_enemy(Player *p, Enemy *e);

bool bullet_collision_floating(Bullet *b, Enemy *e);
bool bullet_collision_minelayer(Bullet *b, Enemy *e);
bool bullet_collision_square_mine(Bullet *b, Enemy *e);
bool bullet_collision_mf_mine(Bullet *b, Enemy *e);
bool bullet_collision_enemy(Bullet *b, Enemy *e);

bool bullet_collision_player(Player *p1, Player *p2);


