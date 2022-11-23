#include "player.h"
#include "enemy.h"

#define drawList igGetBackgroundDrawList_Nil()
void debug_menu_player(Player *p, bool debugMenu)
{
    igBegin("Player", &debugMenu, ImGuiWindowFlags_None);
    igText("Pos : (%f,%f)", p->axis.origin.x, p->axis.origin.y);
    igText("Lives = %d", p->lives);
    igText("Speed = %f", p->speed);
    if (DEBUG_PHYSIC)
    { // && igButton("Display axis", (ImVec2){0, 0}))
        p->displayAxis = !p->displayAxis;
        // if (igButton("Display speed", (ImVec2){0, 0}))
        p->displaySpeed = !p->displaySpeed;
        // if (igButton("Display inertia", (ImVec2){0, 0}))
        p->displayInertia = !p->displayInertia;
        // if (igButton("Display sphere", (ImVec2){0, 0}))
        p->displaySSphere = !p->displaySSphere;
        // if (igButton("Collision Tests", (ImVec2){0, 0}))
        p->collisionTests = !p->collisionTests;
    }
    igEnd();
}
// Spawn player at Point (x,y)
void player_spawn(Player *p, float x, float y)
{
    p->axis = (Axis2){{x, y},
                      {0, -p->size / 25.f}, // 25 px is the base size of the ship
                      {p->size / 25.f, 0}};
    p->inertia = (Vector2){0, 0};
    p->speed = 0;
    p->moveLine = p->axis.x;
    p->targetLine = p->moveLine;
}
// Initialize Player at position(x,y) First time think about setting lives to 3.
Player player_init(float x, float y, float size)
{
    Player p = {0};
    p.lives = 3;
    p.size = size;
    p.spawnPoint = (Point2){x, y};
    player_spawn(&p, x, y);
    return p;
}

// Set the shape of the player
void init_points_player(Player *p)
{
    float x = p->axis.origin.x;
    float y = p->axis.origin.y;
    Point2 origin = p->axis.origin;
    Vector2 axeY = p->axis.x; // initialize Rocket with arrow up
    Vector2 axeX = p->axis.y;
    Point2 point[10] = {
        translatePoint2(origin, multVector2(axeY, 25.f)),                                        // Head   {0,25}
        translatePoint2(origin, multVector2(axeX, -5.f)),                                        //        {0,-5}
        translatePoint2(origin, multVector2(axeX, -8.f)),                                        //        {0,-8}
        translatePoint2(origin, addVector2(multVector2(axeX, -15.f), multVector2(axeY, -20.f))), //        {-15,-20}
        translatePoint2(origin, addVector2(multVector2(axeX, -4.2f), multVector2(axeY, -07.f))), //        {-4.-2,7}
        translatePoint2(origin, multVector2(axeY, -12.f)),                                       // Tail   {0,-12}
        translatePoint2(origin, addVector2(multVector2(axeX, +4.2f), multVector2(axeY, -07.f))), //        {4.2,7}
        translatePoint2(origin, addVector2(multVector2(axeX, +15.f), multVector2(axeY, -20.f))), //        {15,20}
        translatePoint2(origin, multVector2(axeX, 8.f)),                                         //        {0,8}
        translatePoint2(origin, multVector2(axeX, 5.f))};                                        //        {0, 5}

    for (int i = 0; i < 10; i++)
        p->shape[i] = point[i];
}

// Rotate the player
void rotate_player(Player *p, float angle)
{
    p->axis = rotateAxis2(p->axis, angle);
}

// Fire a bullet
void fire_bullet(Player *p, float deltaTime, Point2 maxScreen)
{
    if (p->firecd > 0.25) // 4 bullets/seconds max
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (p->bullets[i].lifespan == 0)
            {
                p->bullets[i] = init_bullet(*p, maxScreen);
                break;
            }
        }
        p->firecd = 0;
    }
}

// Update the player each frame
void update_player(Player *p, float deltaTime, Point2 maxScreen, bool p2, Enemy *e)
{
    if (deltaTime && p->lives >= 0)
    {
        // INPUTS
        if ((igIsKeyDown(ImGuiKey_D) && !p2) || (igIsKeyDown(ImGuiKey_J) && p2))
            turnleft_player(p, deltaTime);
        if ((igIsKeyDown(ImGuiKey_G) && !p2) || (igIsKeyDown(ImGuiKey_L) && p2))
            turnright_player(p, deltaTime);
        if ((igIsKeyDown(ImGuiKey_R) && !p2) || (igIsKeyDown(ImGuiKey_I) && p2))
            accelerate_player(p, deltaTime);
        if ((igIsKeyDown(ImGuiKey_F) && !p2) || (igIsKeyDown(ImGuiKey_K) && p2))
            fire_bullet(p, deltaTime, maxScreen);
        if (((igIsKeyPressed(ImGuiKey_E, 0) || (igIsKeyPressed(ImGuiKey_T, 0))) && !p2) || ((igIsKeyPressed(ImGuiKey_U, 0) || igIsKeyPressed(ImGuiKey_O, 0)) && p2))
            teleport_player(p, maxScreen, e);

        // Collisions
        poly_collision_border_replace(p->shape, &p->axis.origin, 10, p->size, maxScreen);

        p->moveLine = multVector2(p->axis.x, p->size);
        p->targetLine = p->moveLine;
        // Deceleration
        p->speed = normVector2(p->inertia);
        p->inertia = multVector2(p->inertia, 1 - DECELERATION * deltaTime);
        // Displacement
        p->axis = translateAxis2(p->axis, multVector2(p->inertia, deltaTime));
    }
    update_bullet(p, deltaTime, maxScreen);
    p->firecd += deltaTime;
    p->tpcd += deltaTime;
    init_points_player(p);
}

// Collision player with enemy
// test collision mine
bool player_collision_enemy(Player *p, Enemy *e)
{
    if (e->status != ADULT)
        return false;
    Point2 largeBody[3] = {p->shape[0], p->shape[3], p->shape[7]};
    Point2 smallParts[4][3] = {{p->shape[0], p->shape[1], p->shape[9]},    // ARROW
                               {p->axis.origin, p->shape[2], p->shape[3]}, // LEFTWING
                               {p->axis.origin, p->shape[7], p->shape[8]}, // RIGHTWING
                               {p->shape[1], p->shape[5], p->shape[9]}};   // TAIL
    if (sphere_collision_sphere(p->axis.origin, p->size, e->location.origin, get_max_size(e->size, e->type)))
    {
        switch (e->type)
        {
        case (FIREBALL):
            if (sphere_collision_SAT(e->location.origin, get_max_size(e->size, e->type), largeBody, 3))
                for (int i = 0; i < 4; i++)
                    return sphere_collision_SAT(e->location.origin, get_max_size(e->size, e->type), smallParts[i], 3);
        case (FLOATING):
            return player_collision_floating(p, e);
        case (MAGNET_FIRE):
            return player_collision_mf_mine(p, e);
        case (MINELAYER):
            return player_collision_minelayer(p, e);
        default:
            return player_collision_square_mine(p, e);
        }
    }
    return false;
}

// Collision player & floating mine
bool player_collision_floating(Player *p, Enemy *e)
{
    Point2 largeBody[3] = {p->shape[0], p->shape[3], p->shape[7]};
    Point2 smallParts[4][3] = {{p->shape[0], p->shape[1], p->shape[9]},    // ARROW
                               {p->axis.origin, p->shape[2], p->shape[3]}, // LEFTWING
                               {p->axis.origin, p->shape[7], p->shape[8]}, // RIGHTWING
                               {p->shape[1], p->shape[5], p->shape[9]}};   // TAIL
    Point2 largeMine[3] = {e->points[0], e->points[2], e->points[4]};
    Point2 mineParts[4][3];
    if (SAT_collision_SAT(largeMine, 3, largeBody, 3))
    {
        for (int i = 0; i < 3; i++)
        {
            mineParts[3][i] = e->points[i * 2 + 1];       // Internal triangle
            mineParts[i][0] = e->points[i * 2 + 1];       //
            mineParts[i][1] = e->points[(i * 2 + 2) % 6]; // External triangles
            mineParts[i][2] = e->points[(i * 2 + 3) % 6]; //
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 3))
                    return true;
        }
    }
    return false;
}

// Collision player & minelayer
bool player_collision_minelayer(Player *p, Enemy *e)
{
    Point2 largeBody[3] = {p->shape[0], p->shape[3], p->shape[7]};
    Point2 smallParts[4][3] = {{p->shape[0], p->shape[1], p->shape[9]},    // ARROW
                               {p->axis.origin, p->shape[2], p->shape[3]}, // LEFTWING
                               {p->axis.origin, p->shape[7], p->shape[8]}, // RIGHTWING
                               {p->shape[1], p->shape[5], p->shape[9]}};   // TAIL
    Point2 largeMine[5] = {e->points[0], e->points[2], e->points[3], e->points[6], e->points[7]};
    Point2 mineParts[3][3];
    if (SAT_collision_SAT(largeMine, 5, largeBody, 3))
    {
        for (int i = 0; i < 3; i++)
        {
            mineParts[0][i] = e->points[3 + i]; // Big triangle centered
            mineParts[1][i] = e->points[i + 1]; // Left wing
            mineParts[2][i] = e->points[i + 6]; // Right Wing
        }
        mineParts[0][0] = e->points[0];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 3))
                    return true;
        }
    }
    return false;
}

// Collision player & square-shaped mine (except from Magnetic-fire)
bool player_collision_square_mine(Player *p, Enemy *e)
{
    Point2 largeBody[3] = {p->shape[0], p->shape[3], p->shape[7]};
    Point2 smallParts[4][3] = {{p->shape[0], p->shape[1], p->shape[9]},    // ARROW
                               {p->axis.origin, p->shape[2], p->shape[3]}, // LEFTWING
                               {p->axis.origin, p->shape[7], p->shape[8]}, // RIGHTWING
                               {p->shape[1], p->shape[5], p->shape[9]}};   // TAIL
    Point2 largeMine[4] = {e->points[0], e->points[2], e->points[4], e->points[6]};
    Point2 mineParts[4][3];
    Point2 interior_square[4];
    for (int i = 0; i < 4; i++)
        largeMine[i] = e->points[2 * i];
    if (SAT_collision_SAT(largeMine, 4, largeBody, 3))
    {
        for (int i = 0; i < 4; i++)
        {
            interior_square[i] = e->points[(2 * i + 1)];
            mineParts[i][0] = e->points[(2 * i + 1)];
            mineParts[i][1] = e->points[(2 * i + 2) % 8];
            mineParts[i][2] = e->points[(2 * i + 3) % 8];
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 4))
                    return true;
            }
            if (SAT_collision_SAT(smallParts[i], 3, interior_square, 4))
                return true;
        }
    }
    return false;
}

// Collision player & Magnetic-fire mine
bool player_collision_mf_mine(Player *p, Enemy *e)
{
    Point2 largeBody[3] = {p->shape[0], p->shape[3], p->shape[7]};
    Point2 smallParts[4][3] = {{p->shape[0], p->shape[1], p->shape[9]},    // ARROW
                               {p->axis.origin, p->shape[2], p->shape[3]}, // LEFTWING
                               {p->axis.origin, p->shape[7], p->shape[8]}, // RIGHTWING
                               {p->shape[1], p->shape[5], p->shape[9]}};   // TAIL
    Point2 largeMine[4] = {e->points[0], e->points[2], e->points[4], e->points[6]};
    Point2 mineParts[4][3];
    for (int i = 0; i < 4; i++)
        largeMine[i] = e->points[2 * i];
    if (SAT_collision_SAT(largeMine, 4, largeBody, 3))
    {
        for (int i = 0; i < 4; i++)
        {
            mineParts[i][0] = e->points[2 * i];
            mineParts[i][1] = e->points[2 * i + 1];
            mineParts[i][2] = e->location.origin;
        }
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 4))
                    return true;
            }
        }
    }
    return false;
}

// Create a bullet
Bullet init_bullet(Player p, Point2 maxScreen)
{
    Bullet b;
    b.direction = multVector2(p.targetLine, 1 + (p.speed / MAX_SPEED_SHIP)); // Keeps the ship's speed
    b.location = addVector2(p.axis.origin, p.targetLine);
    b.size = p.size / 6;
    b.lifespan = (maxScreen.y / p.size) * 4 / 5;
    return b;
}
// Update ALL the bullets of a player
void update_bullet(Player *p, float deltaTime, Point2 maxScreen)
{
    for (int i = 0; i < MAX_BULLETS; i++)
        if (p->bullets[i].lifespan)
            update_one_bullet(&p->bullets[i], deltaTime, maxScreen);
}
// Bullet evolution
void update_one_bullet(Bullet *b, float deltaTime, Point2 maxScreen)
{
    b->location = addVector2(b->location, multVector2(b->direction, 30 * deltaTime));
    sphere_collision_border_replace(&b->location, b->size, maxScreen);
    b->lifespan -= deltaTime * 30;
    if (b->lifespan < 0)
        b->lifespan = 0;
}

// Collision bullet with enemy
bool bullet_collision_enemy(Bullet *b, Enemy *e)
{
    if (e->status != ADULT)
        return false;
    if (sphere_collision_sphere(b->location, b->size, e->location.origin, get_max_size(e->size, e->type)))
    {
        switch (e->type)
        {
        case (FIREBALL):
            if (sphere_collision_sphere(e->location.origin, get_max_size(e->size, e->type), b->location, b->size))
            {
                b->lifespan = 0;
                return true;
            }
        case (FLOATING):
            return bullet_collision_floating(b, e);
        case (MAGNET_FIRE):
            return bullet_collision_mf_mine(b, e);
        case (MINELAYER):
            return bullet_collision_minelayer(b, e);
        default:
            return bullet_collision_square_mine(b, e);
        }
    }
    return false;
}
// Collision bullet & floating mine
bool bullet_collision_floating(Bullet *b, Enemy *e)
{
    Point2 largeMine[3] = {e->points[0], e->points[2], e->points[4]};
    Point2 mineParts[4][3];
    if (sphere_collision_SAT(b->location, b->size, largeMine, 3))
    {
        for (int i = 0; i < 3; i++)
        {
            mineParts[3][i] = e->points[i * 2 + 1];       // Internal triangle
            mineParts[i][0] = e->points[i * 2 + 1];       //
            mineParts[i][1] = e->points[(i * 2 + 2) % 6]; // External Triangles
            mineParts[i][2] = e->points[(i * 2 + 3) % 6]; //
        }
        for (int i = 0; i < 4; i++)
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 3))
            {
                b->lifespan = 0;
                return true;
            }
    }
    return false;
}
// Collision bullet & minelayer
bool bullet_collision_minelayer(Bullet *b, Enemy *e)
{
    Point2 largeMine[5] = {e->points[0], e->points[2], e->points[3], e->points[6], e->points[7]};
    Point2 mineParts[3][3];
    if (sphere_collision_SAT(b->location, b->size, largeMine, 5))
    {
        for (int i = 0; i < 3; i++)
        {
            mineParts[0][i] = e->points[3 + i]; // Big center Triangle
            mineParts[1][i] = e->points[i + 1]; // LeftWing
            mineParts[2][i] = e->points[i + 6]; // RightWing
        }
        mineParts[0][0] = e->points[0];
        for (int i = 0; i < 3; i++)
        {
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 3))
            {
                b->lifespan = 0;
                return true;
            }
        }
    }
    return false;
}
// Collision bullet & square-shaped mine (except from Magnetic-fire)
bool bullet_collision_square_mine(Bullet *b, Enemy *e)
{
    Point2 largeMine[4] = {e->points[0], e->points[2], e->points[4], e->points[6]};
    Point2 mineParts[4][3];
    Point2 interior_square[4];
    for (int i = 0; i < 4; i++)
        largeMine[i] = e->points[2 * i];
    if (sphere_collision_SAT(b->location, b->size, largeMine, 4))
    {
        for (int i = 0; i < 4; i++)
        {
            interior_square[i] = e->points[(2 * i + 1)];
            mineParts[i][0] = e->points[(2 * i + 1)];
            mineParts[i][1] = e->points[(2 * i + 2) % 8];
            mineParts[i][2] = e->points[(2 * i + 3) % 8];
        }
        for (int i = 0; i < 4; i++)
        {
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 4))
            {
                b->lifespan = 0;
                return true;
            }
        }
        if (sphere_collision_SAT(b->location, b->size, interior_square, 4))
        {
            b->lifespan = 0;
            return true;
        }
    }
    return false;
}
// Collision bullet & Magnetic-fire mine
bool bullet_collision_mf_mine(Bullet *b, Enemy *e)
{
    Point2 largeMine[4] = {e->points[0], e->points[2], e->points[4], e->points[6]};
    Point2 mineParts[4][3];
    for (int i = 0; i < 4; i++)
        largeMine[i] = e->points[2 * i];
    if (sphere_collision_SAT(b->location, b->size, largeMine, 4))
    {
        for (int i = 0; i < 4; i++)
        {
            mineParts[i][0] = e->points[2 * i];     // Summit big square
            mineParts[i][1] = e->points[2 * i + 1]; // Summit small square
            mineParts[i][2] = e->location.origin;   // Center of square
        }
        for (int i = 0; i < 4; i++)
        {
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 4))
            {
                b->lifespan = 0;
                return true;
            }
        }
    }
    return false;
}

// Collision bullet with player
// test collision (p1 = shooter, p2 = target)
bool bullet_collision_player(Player *p1, Player *p2)
{
    Point2 largeBody[3] = {p2->shape[0], p2->shape[3], p2->shape[7]};
    Point2 smallParts[4][3] = {{p2->shape[0], p2->shape[1], p2->shape[9]},    // ARROW
                               {p2->axis.origin, p2->shape[2], p2->shape[3]}, // LEFTWING
                               {p2->axis.origin, p2->shape[7], p2->shape[8]}, // RIGHTWING
                               {p2->shape[1], p2->shape[5], p2->shape[9]}};   // TAIL
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (p1->bullets[i].lifespan > 0 && sphere_collision_sphere(p1->bullets[i].location, p1->bullets[i].size, p2->axis.origin, p2->size))
        {
            for (int j = 0; j < 4; j++)
                if (sphere_collision_SAT(p1->bullets[i].location, p1->bullets[i].size, smallParts[j], 3))
                {
                    p1->bullets[i].lifespan = 0;
                    return true;
                }
        }
    }
    return false;
}
// Reset the bullets (*p2 = NULL if 1p game)
void bullets_terminate(Player *p1, Player *p2)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        p1->bullets[i].lifespan = 0;
        if (!p2)
            p2->bullets[i].lifespan = 0;
    }
}
// Turn the player to the left igIsKeyDown(ImGuiKey_D)
void turnleft_player(Player *p, float deltaTime)
{
    rotate_player(p, -M_PI * deltaTime);
}

// Turn the player to the right igIsKeyDown(ImGuiKey_G)
void turnright_player(Player *p, float deltaTime)
{
    rotate_player(p, M_PI * deltaTime);
}
// Add ACCELERATION to speed
void accelerate_player(Player *p, float deltaTime)
{
    if (p->speed < MAX_SPEED_SHIP) // Never exceed MAX_SPEED
    {
        p->speed += ACCELERATION * deltaTime;
    }
    else
        p->inertia = multVector2(p->inertia, MAX_SPEED_SHIP / p->speed);                       // Re-Calculate to keep Max Speed
    p->inertia = addVector2(p->inertia, multVector2(p->targetLine, ACCELERATION * deltaTime)); // Update inertia
    p->moveLine = p->targetLine;
}
// Teleport player at a random position, should check the collisions first
void teleport_player(Player *p, Point2 maxScreen, Enemy *e)
{
    if (p->tpcd >= 3)
    {
        Point2 newPos;
        srand(time(NULL));
        newPos.x = (int)(p->axis.origin.x + rand()) % (int)(maxScreen.x - 2 * p->size);
        newPos.y = (int)(p->axis.origin.y + rand()) % (int)(maxScreen.y - 2 * p->size);
        player_spawn_check(p, newPos, maxScreen, e); // Make sure we don't spawn on an enemy
        p->tpcd = 0;
    }
}
// Check collisions before spawn
void player_spawn_check(Player *p, Point2 newLocation, Point2 maxScreen, Enemy *e)
{
    srand(time(NULL));
    bool collision = false;
    do
    {
        newLocation.x = (int)(p->axis.origin.x + rand()) % (int)(maxScreen.x - 2 * p->size) + p->size;
        newLocation.y = (int)(p->axis.origin.y + rand()) % (int)(maxScreen.y - 2 * p->size) + p->size;
        for (int i = 0; i < MAX_ENEMY; i++)
            if (e[i].status == ADULT)
            {
                collision = sphere_collision_sphere(newLocation, 2 * p->size, e[i].location.origin, get_max_size(e[i].size, e[i].type));
                break;
            }
    } while (collision);
    if (!collision)
    {
        player_spawn(p, newLocation.x, newLocation.y);
        return;
    }
}