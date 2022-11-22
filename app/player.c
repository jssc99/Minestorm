#include "player.h"
#include "enemy.h"

bool DEBUG_PLAYER = 1;

#define drawList igGetBackgroundDrawList_Nil()

void debug_menu_player(Player *p, bool debugMenu)
{
    igBegin("Player", &debugMenu, ImGuiWindowFlags_None);
    igText("Pos : (%f,%f)", p->axis.origin.x, p->axis.origin.y);
    igText("Lives = %d", p->lives);
    igText("Speed = %f", p->speed);
    if (DEBUG_PLAYER && igButton("Display axis", (ImVec2){0, 0}))
        p->displayAxis = !p->displayAxis;
    if (DEBUG_PLAYER && igButton("Display speed", (ImVec2){0, 0}))
        p->displaySpeed = !p->displaySpeed;
    if (DEBUG_PLAYER && igButton("Display inertia", (ImVec2){0, 0}))
        p->displayInertia = !p->displayInertia;
    if (DEBUG_PLAYER && igButton("Display sphere", (ImVec2){0, 0}))
        p->displaySSphere = !p->displaySSphere;
    if (DEBUG_PLAYER && igButton("Collision Tests", (ImVec2){0, 0}))
        p->collisionTests = !p->collisionTests;

    igEnd();
}
// Spawn player at Point (x,y)
void player_spawn(Player *p, float x, float y)
{
    p->axis = (Axis2){{x, y},
                      {0, -1},
                      {1, 0}};
    p->inertia = (Vector2){0, 0};
    p->speed = 0;
    // p->moveLine = multVector2(p->axis.x, p->size);
    p->axis.x = multVector2(p->axis.x, p->size / 25.f); // 25 px is the base size of the ship
    p->axis.y = multVector2(p->axis.y, p->size / 25.f);
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
    // p.axis.x = multVector2(p.axis.x, p.size / 25.f); // 25 px is the base size of the ship
    // p.axis.y = multVector2(p.axis.y, p.size / 25.f);
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
        translatePoint2(origin, multVector2(axeX, 8.f)),                                         //        {0, 8}
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
    if (p->firecd > 0.25)
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (p->bullets[i].lifespan == 0)
            {
                p->bullets[i] = init_bullet(*p);
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

        // else
        {
            // p->targetLine = p->axis.x; // multVector2(addVector2(p->axis.x,p->axis.y), 0.5);
            p->moveLine = multVector2(p->axis.x, p->size);
            p->targetLine = p->moveLine;
            // Deceleration
            p->speed = normVector2(p->inertia);
            p->inertia = multVector2(p->inertia, 1 - DECELERATION * deltaTime);
            // p->speed *= DECELERATION;
            // Displacement
            p->axis = translateAxis2(p->axis, multVector2(p->inertia, deltaTime));
        }
        update_bullet(p, deltaTime, maxScreen);
        p->firecd += deltaTime;
        p->tpcd += deltaTime;
        init_points_player(p);
    }
}

// Collision player with enemy
// test collision mine
bool player_collision_enemy(Player *p, Enemy *e)
{
    // Player player = *p;
    if (e->status != ADULT)
        return false;
    // bool outside;
    Point2 largeBody[3] = {p->shape[0], p->shape[3], p->shape[7]};
    Point2 largeMine[5];
    Point2 smallParts[4][3] = {{p->shape[0], p->shape[1], p->shape[9]},    // ARROW
                               {p->axis.origin, p->shape[2], p->shape[3]}, // LEFTWING
                               {p->axis.origin, p->shape[7], p->shape[8]}, // RIGHTWING
                               {p->shape[1], p->shape[5], p->shape[9]}};   // TAIL
    Point2 mineParts[5][3];

    if (sphere_collision_sphere(p->axis.origin, p->size, e->location.origin, get_max_size(e->size, e->type)))
    {
        switch (e->type)
        {
        case (FIREBALL):
            for (int i = 0; i < 4; i++)
                if (sphere_collision_SAT(e->location.origin, get_max_size(e->size, e->type), smallParts[i], 3))
                {
                    return true;
                }
            break;
        case (FLOATING):
            return player_collision_floating(p, e);
            /*    largeMine[0] = e->points[0];
                largeMine[1] = e->points[2];
                largeMine[2] = e->points[4];
                if (SAT_collision_SAT(largeMine, 3, largeBody, 3))
                {
                    for (int i = 0; i < 3; i++)
                    {
                        mineParts[i][0] = e->points[i * 2];
                        mineParts[i][1] = e->points[i * 2 + 1];
                        mineParts[i][2] = e->points[(i * 2 + 2) % 6];
                    }
                    for (int i = 0; i < 3 && !outside; i++)
                    {
                        for (int j = 0; j < 3 && !outside; j++)
                            outside = SAT_collision_SAT(smallParts[i], 3, mineParts[j], 3);
                    }
                    if (!outside)
                        collision = true;
                }*/
            break;
        case (MAGNET_FIRE):
            return player_collision_mf_mine(p, e);
            break;
        case (MINELAYER):
            return player_collision_minelayer(p, e);
            /*
                for (int i = 0; i < 5; i++)
                    largeMine[i] = e->points[i];
                if (SAT_collision_SAT(largeMine, 5, largeBody, 3))
                {
                    for (int i = 0; i < 3; i++)
                    {
                        mineParts[1][i] = e->points[i + 1];
                        mineParts[2][i] = e->points[i + 3];
                        mineParts[3][i] = e->points[i + 4];
                        mineParts[4][i] = e->points[(i + 7) % 9];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                            if (SAT_collision_SAT(smallParts[i], 3, mineParts[2 - j], 3))
                            {
                                return true;
                                break;
                            }
                    }
                    break;
                }*/
            break;
        default:
            return player_collision_square_mine(p, e);
            /*for (int i = 0; i < 4; i++)
                largeMine[i] = e->points[2 * i];
            if (SAT_collision_SAT(largeMine, 4, largeBody, 3))
            {
                for (int i = 0; i < 4; i++)
                {
                    mineParts[i][0] = e->points[i * 2];
                    mineParts[i][1] = e->points[i * 2 + 1];
                    mineParts[i][2] = e->points[(i * 2 + 2) % 6];
                }
                for (int i = 0; i < 3 && !outside; i++)
                {
                    for (int j = 0; j < 4 && !outside; j++)
                        outside = SAT_collision_SAT(smallParts[i], 3, mineParts[j], 4);
                    if (outside)
                        break;
                }
                if (!outside)
                    collision = true;
            }*/
            break;
        }
    } /*
     if (collision)
     {
         player_spawn(p, p->spawnPoint.x, p->spawnPoint.y);
         p->lives--;
         e->status = DEAD;
     }*/
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
            mineParts[3][i] = e->points[i * 2 + 1]; // Internal triangle
            mineParts[i][0] = e->points[i * 2 + 1];
            mineParts[i][1] = e->points[(i * 2 + 2) % 6];
            mineParts[i][2] = e->points[(i * 2 + 3) % 6];
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
    // Point2 internalRectangle[4] = {e->points[2], e->points[4], e->points[5], e->points[7]};
    if (SAT_collision_SAT(largeMine, 5, largeBody, 3))
    {
        for (int i = 0; i < 3; i++)
        {
            mineParts[0][i] = e->points[3 + i];
            mineParts[1][i] = e->points[i + 1];
            mineParts[2][i] = e->points[i + 6];
        }
        mineParts[0][0] = e->points[0];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 3))
                    return true;
            // if (SAT_collision_SAT(smallParts[i], 3, internalRectangle, 4))
            //   return true;
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
            //            mineParts[i][0] = e->points[i * 2];
            //          mineParts[i][1] = e->points[i * 2 + 1];
            //        mineParts[i][2] = e->points[(i * 2 + 2) % 6];
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
Bullet init_bullet(Player p)
{
    Bullet b;
    b.direction = multVector2(p.targetLine, 1 + (p.speed / MAX_SPEED_SHIP)); // Keeps the ship's speed
    b.location = addVector2(p.axis.origin, p.targetLine);
    b.size = p.size / 10;
    b.lifespan = (800 / p.size) * 4 / 5;
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
// TODO: void update_bullet(Bullet* b, float deltaTime)
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
    Point2 largeMine[5];
    Point2 mineParts[5][3];

    if (sphere_collision_sphere(b->location, b->size, e->location.origin, get_max_size(e->size, e->type)))
    {
        switch (e->type)
        {
        case (FIREBALL):
            if (sphere_collision_sphere(e->location.origin, get_max_size(e->size, e->type), b->location, b->size))
                return true;
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
            mineParts[3][i] = e->points[i * 2 + 1]; // Internal triangle
            mineParts[i][0] = e->points[i * 2 + 1];
            mineParts[i][1] = e->points[(i * 2 + 2) % 6];
            mineParts[i][2] = e->points[(i * 2 + 3) % 6];
        }
        for (int i = 0; i < 4; i++)
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 3))
                return true;
    }
    return false;
}

// Collision bullet & minelayer
bool bullet_collision_minelayer(Bullet *b, Enemy *e)
{

    Point2 largeMine[5] = {e->points[0], e->points[2], e->points[3], e->points[6], e->points[7]};
    Point2 mineParts[3][3];
    // Point2 internalRectangle[4] = {e->points[2], e->points[4], e->points[5], e->points[7]};
    if (sphere_collision_SAT(b->location, b->size, largeMine, 5))
    {
        for (int i = 0; i < 3; i++)
        {
            mineParts[0][i] = e->points[3 + i];
            mineParts[1][i] = e->points[i + 1];
            mineParts[2][i] = e->points[i + 6];
        }
        mineParts[0][0] = e->points[0];
        for (int i = 0; i < 3; i++)
        {
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 3))
                return true;
            // if (SAT_collision_SAT(smallParts[i], 3, internalRectangle, 4))
            //   return true;
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
            //            mineParts[i][0] = e->points[i * 2];
            //          mineParts[i][1] = e->points[i * 2 + 1];
            //        mineParts[i][2] = e->points[(i * 2 + 2) % 6];
        }
        for (int i = 0; i < 4; i++)
        {
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 4))
                return true;
        }
        if (sphere_collision_SAT(b->location, b->size, interior_square, 4))
            return true;
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
            mineParts[i][0] = e->points[2 * i];
            mineParts[i][1] = e->points[2 * i + 1];
            mineParts[i][2] = e->location.origin;
        }
        for (int i = 0; i < 4; i++)
        {
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 4))
                return true;
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
                    return true;
                }
        }
    }
    return false;
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
    if (p->speed < MAX_SPEED_SHIP)
    {
        p->speed += ACCELERATION * deltaTime;
    }
    else
        p->inertia = multVector2(p->inertia, MAX_SPEED_SHIP / p->speed);
    p->inertia = addVector2(p->inertia, multVector2(p->targetLine, ACCELERATION * deltaTime));
    p->moveLine = p->targetLine;
}
// Teleport player at a random position, should check the collisions first
void teleport_player(Player *p, Point2 maxScreen, Enemy *e)
{
    if (p->tpcd >= 0 /*3*/)
    {
        // bool collision = false;
        Point2 newPos;
        // do
        //{
        srand(time(NULL)); // i++ ?
        newPos.x = (int)(p->axis.origin.x + rand()) % (int)(maxScreen.x - 2 * p->size);
        newPos.y = (int)(p->axis.origin.y + rand()) % (int)(maxScreen.y - 2 * p->size);
        /*    for (int i = 0; i < MAX_ENEMY; i++)
                if (e->status == ADULT)
                {
                    collision = sphere_collision_sphere(newPos, 2 * p->size, e[i].location.origin, get_max_size(e[i].size, e[i].type));
                    break;
                }
        } while (collision);

       // player_spawn(p, p->size + newPos.x, p->size + newPos.y);*/
        player_spawn_check(p, newPos, maxScreen, e);
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

// Draw a circle or a polygon with Canvas (obsolete)
void draw_circle(Point2 *cBox, Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color)
{
    Point2 point[sides];
    point[0].x = radius * sinf(angleOffset) + center.x;
    point[0].y = radius * cosf(angleOffset) + center.y;
    cvPathLineTo(point[0].x, point[0].y);

    float angle = M_PI * 2 / (float)sides;
    float baseX = point[0].x;
    float baseY = point[0].y;
    for (unsigned int i = 1; i < sides; i++)
    {
        float c = cosf(angle * i);
        float s = sinf(angle * i);
        point[i].x = (baseX - center.x) * c - (baseY - center.y) * s + center.x;
        point[i].y = (baseX - center.x) * s + (baseY - center.y) * c + center.y;
        cvPathLineTo(point[i].x, point[i].y);
    }
    cvPathStroke(color, 1);

    if (cBox)
    {
        for (unsigned int i = 0; i < sides; i++)
            cBox[i] = point[i];
    }
}

void test_collision(Player player1, Player player2, ImVec2 mousePos, Enemy e)
{
    // Rectangle collision
    // bool collision = sphere_collision_rectangle((Point2){mousePos.x, mousePos.y}, 2, 460, 360, 540, 440);
    // draw_circle(NULL, (Point2){500, 400}, 4, 40 * sqrtf(2), M_PI / 4, CV_COL32_WHITE);

    // Sat collision
    /*
    Point2 rectangle[4] = {{400.f, 350.f}, {400.f, 450.f}, {550.f, 450.f}, {550.f, 350.f}};
    for (int i = 0; i < 4; i++)
    {
        rectangle[i] = rotatePoint2((Point2){500, 400}, rectangle[i], M_PI / 3);
    }
    /
    Point2 quad[4] = {{390.f, 350.f}, {410.f, 440.f}, {520.f, 440.f}, {540.f, 380.f}};
    Point2 triangle[3] = {{520.f, 390.f}, {480.f, 390.f}, {500.f, 430.f}};
    Point2 poly[6] = {{480.f, 320.f}, {410.f, 350.f}, {390.f, 400.f}, {410.f, 440.f}, {520.f, 470.f}, {540.f, 380.f}};
    for (int i = 0; i < 6; i++)
    {
        poly[i] = rotatePoint2((Point2){500, 400}, poly[i], M_PI / 6);
    }

    Point2 poly_[6];
    for (int i = 0; i < 6; i++)
        poly_[i] = poly[6 - 1 - i];
    Point2 poly_m[6];
    for (int i = 0; i < 6; i++)
    {
        poly_m[i].x = poly[i].x + mousePos.x - 500.f;
        poly_m[i].y = poly[i].y + mousePos.y - 400.f;
    }
    for (int i = 0; i < 6; i++)
    {
        cvAddLine(poly_m[i].x, poly_m[i].y, poly_m[(i + 1) % 6].x, poly_m[(i + 1) % 6].y, CV_COL32(255, 0, 0, 255));
    }
    draw_circle(NULL, circumcenterTriangle(triangle[1], triangle[2], triangle[0]), 50, 4 * 3 * sqrtf(3), 0, CV_COL32(0, 0, 255, 255));
        for (int i = 0; i < 4; i++)
            {
                cvAddLine(quad[i].x, quad[i].y, quad[(i + 1) % 4].x, quad[(i + 1) % 4].y, CV_COL32(255, 0, 0, 255));
            }

       bool collision = sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, quad, 4);

    for (int i = 0; i < 3; i++)
    {
        cvAddLine(triangle[i].x, triangle[i].y, triangle[(i + 1) % 3].x, triangle[(i + 1) % 3].y, CV_COL32(255, 0, 0, 255));
    }
    bool collision = sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, triangle, 3);

    cvAddLine(poly[0].x, poly[0].y, poly[(0 + 1) % 6].x, poly[(0 + 1) % 6].y, CV_COL32(255, 0, 255, 255));
    for (int i = 1; i < 6; i++)
    {
        cvAddLine(poly[i].x, poly[i].y, poly[(i + 1) % 6].x, poly[(i + 1) % 6].y, CV_COL32(255, 0, 0, 255));
    }
    Point2 largeBody[3] = {player1.shape[0], player1.shape[3], player1.shape[7]}; // exclude 2 points :(
    Point2 arrow[3] = {player1.shape[0], player1.shape[1], player1.shape[9]};
    Point2 leftWing[3] = {player1.axis.origin, player1.shape[2], player1.shape[3]};
    Point2 rightWing[3] = {player1.axis.origin, player1.shape[7], player1.shape[8]};
    Point2 tail[3] = {player1.shape[1], player1.shape[5], player1.shape[8]};


    Point2 largeBody[3] = LARGEBODY(player1);
    Point2 smallParts[4][3] = {ARROW(player1), LEFTWING(player1), RIGHTWING(player1), TAIL(player1)};
    Point2 largeBody2[3] = LARGEBODY(player2);
    Point2 arrow2[3] = ARROW(player2);
    Point2 leftWing2[3] = LEFTWING(player2);
    Point2 rightWing2[3] = RIGHTWING(player2);
    Point2 tail2[3] = TAIL(player2);
    bool collision = sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, smallParts[3], 3);
        bool collision = (sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, arrow1, 3)        //
                          || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, leftWing1, 3)  //
                          || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, rightWing1, 3) //
                          || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, tail1, 3));
        bool collision = SAT_collision_SAT(largeBody1, 3, largeBody2, 3);

    bool collision;
    for (int i = 0; i < 4; i++)
    {
        if (!collision)
        {
            collision = (SAT_collision_SAT(arrow2, 3, smallParts[i], 3)        //
                         || SAT_collision_SAT(leftWing2, 3, smallParts[i], 3)  //
                         || SAT_collision_SAT(rightWing2, 3, smallParts[i], 3) //
                         || SAT_collision_SAT(tail2, 3, smallParts[i], 3));
        }
        else
            break;
    }

    // bool collision = SAT_collision_SAT(poly_m, 6, poly, 6);
    // bool collision = sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, rectangle,4);
    for (int i = 0; i < 3; i++)
    {
        printf("Points [%d], = (%f,%f)\n", i, &player1.shape[i].x, &player1.shape[i].y);
    }
    */
    // printf("min = %f, max = %f, x = %f, y = %f\n", player1.sat->min, player1.sat->max, player1.sat->normal.x, player1.sat->normal.y);
    bool collision = false;
    // switch (e.type)

    // case /* constant-expression */:
    /* code */
    //    break;

    // default:

    Point2 largeBody[3] = {player1.shape[0], player1.shape[3], player1.shape[7]};
    Point2 smallParts[4][3] = {{player1.shape[0], player1.shape[1], player1.shape[9]},    // ARROW
                               {player1.axis.origin, player1.shape[2], player1.shape[3]}, // LEFTWING
                               {player1.axis.origin, player1.shape[7], player1.shape[8]}, // RIGHTWING
                               {player1.shape[1], player1.shape[5], player1.shape[9]}};   // TAIL
    Point2 largeMine[3] = {e.points[0], e.points[2], e.points[4]};
    Point2 mineParts[4][3];
    ImVec2 v1;
    ImVec2 v2;
    for (int i = 0; i < 3; i++)
        largeMine[i] = e.points[2 * i];
    if (sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, largeMine, 3))
    {
        collision = true;
        ImVec2 v1;
        ImVec2 v2;
        for (int i = 0; i < 4; i++)
        {
            Point2 largeMine[3] = {e.points[0], e.points[2], e.points[4]};
            Point2 mineParts[3][3];
            if (SAT_collision_SAT(largeMine, 3, largeBody, 3))
            {
                for (int i = 0; i < 3; i++)
                {
                    mineParts[i][0] = e.points[i * 2];
                    mineParts[i][1] = e.points[i * 2 + 1];
                    mineParts[i][2] = e.points[(i * 2 + 2) % 6];
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                        if (SAT_collision_SAT(smallParts[i], 3, mineParts[2 - j], 3))
                            collision = true;
                }
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 4; j++)
                        if (sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, mineParts[j], 3))
                        {
                            collision = false;
                            break;
                        }
                }
            }
            /*
                        mineParts[i][2] = e.points[2 * i];
                        mineParts[i][1] = e.points[(2 * i + 2) % 8];
                        mineParts[i][0] = e.points[(2 * i + 1) % 8];
                        v1.x = e.points[i].x;
                        v1.y = e.points[i].y;
                        v2.x = e.points[i + 4].x;
                        v2.y = e.points[i + 4].y;
                        ImDrawList_AddCircleFilled(drawList, v1, 2, CV_COL32(i * 80, 255 / (i + 1), 255, 255), 20);
                        ImDrawList_AddCircleFilled(drawList, v2, 2, CV_COL32(255, 255 / (i + 1), i * 80, 255), 20);
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 4; j++)
                            if (sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, mineParts[j], 3))
                            {
                                collision = false;
                                break;
                            }
                    }
                }*/
            // if (sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, largeBody, 3))
            //     collision = true;
            draw_circle(NULL, (Point2){mousePos.x, mousePos.y}, 50, 2, 0, CV_COL32((255 * collision), 0, 0, 255));
            // break;
        }
    }
}