#pragma once
#include "physics.h"
#include "enemy.h"
#include <stdlib.h>
#include <time.h>

//#include "draw.c"
// Player separation
#define LARGEBODY(p) {p.shape[0], p.shape[3], p.shape[7]} // Surrounding triangle
#define ARROW(p) {p.shape[0], p.shape[1], p.shape[9]} 
#define LEFTWING(p) {p.axis.origin, p.shape[2], p.shape[3]}
#define RIGHTWING(p) {p.axis.origin, p.shape[7], p.shape[8]}
#define TAIL(p) {p.shape[1], p.shape[5], p.shape[9]}
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
    Point2 spawnPoint;
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    Vector2 inertia;
    Point2 shape[10];
    float size;
    float speed;
    float firecd;
    float tpcd;
    int lives;
    bool displayAxis;
    bool displaySpeed;
    bool displayInertia;
    bool displaySSphere;
    bool collisionTests;
    Bullet bullets[MAX_BULLETS]; // Rename
} Player;

// Draw
void draw_circle(Point2 *cBox, Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color);
void draw_player(Player p, int playerNb);
void draw_debug_player(Player *p);
void draw_bullet(Point2 center, float radius, unsigned int color);
void draw_players_bullets(Player *p);

// Player
Player player_init(float x, float y, float size);
void init_points_player(Player *p);
void player_spawn(Player *p, float x, float y);
void player_spawn_check(Player *p, Point2 newLocation, Point2 maxScreen, Enemy *e);
void rotate_player(Player *p, float angle);
void update_player(Player *p, float deltaTime, Point2 maxScreen, bool p2, Enemy* e);
void turnleft_player(Player *p, float deltaTime);
void turnright_player(Player *p, float deltaTime);
void accelerate_player(Player *p, float deltaTime);
void teleport_player(Player *p, Point2 maxScreen, Enemy* e);
void debug_menu_player(Player *p, bool debugMenu);

// Bullet
Bullet init_bullet(Player p);
void update_one_bullet(Bullet *b, float deltaTime, Point2 maxScreen);
void update_bullet(Player *p, float deltaTime, Point2 maxScreen);
void fire_bullet(Player *p, float deltaTime, Point2 maxScreen);

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


