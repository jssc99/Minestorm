#include "player.h"
#include "enemy.h"

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
            if (sphere_collision_SAT(e->location.origin, get_max_size(e->size, e->type), largeBody, 3,p->hideCollisionBox))
                for (int i = 0; i < 4; i++)
                    return sphere_collision_SAT(e->location.origin, get_max_size(e->size, e->type), smallParts[i], 3,p->hideCollisionBox);
        return false;
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
    if (SAT_collision_SAT(largeMine, 3, largeBody, 3,p->hideCollisionBox))
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
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 3,p->hideCollisionBox))
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
    if (SAT_collision_SAT(largeMine, 5, largeBody, 3,p->hideCollisionBox))
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
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 3,p->hideCollisionBox))
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
    if (SAT_collision_SAT(largeMine, 4, largeBody, 3,p->hideCollisionBox))
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
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 4,p->hideCollisionBox))
                    return true;
            }
            if (SAT_collision_SAT(smallParts[i], 3, interior_square, 4,p->hideCollisionBox))
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
    if (SAT_collision_SAT(largeMine, 4, largeBody, 3,p->hideCollisionBox))
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
                if (SAT_collision_SAT(smallParts[i], 3, mineParts[j], 4,p->hideCollisionBox))
                    return true;
            }
        }
    }
    return false;
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
            return false;
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
    if (sphere_collision_SAT(b->location, b->size, largeMine, 3,0))
    {
        for (int i = 0; i < 3; i++)
        {
            mineParts[3][i] = e->points[i * 2 + 1];       // Internal triangle
            mineParts[i][0] = e->points[i * 2 + 1];       //
            mineParts[i][1] = e->points[(i * 2 + 2) % 6]; // External Triangles
            mineParts[i][2] = e->points[(i * 2 + 3) % 6]; //
        }
        for (int i = 0; i < 4; i++)
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 3,0))
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
    if (sphere_collision_SAT(b->location, b->size, largeMine, 5, 0))
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
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 3, 0))
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
    if (sphere_collision_SAT(b->location, b->size, largeMine, 4,0))
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
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 4,0))
            {
                b->lifespan = 0;
                return true;
            }
        }
        if (sphere_collision_SAT(b->location, b->size, interior_square, 4,0))
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
    if (sphere_collision_SAT(b->location, b->size, largeMine, 4, 0))
    {
        for (int i = 0; i < 4; i++)
        {
            mineParts[i][0] = e->points[2 * i];     // Summit big square
            mineParts[i][1] = e->points[2 * i + 1]; // Summit small square
            mineParts[i][2] = e->location.origin;   // Center of square
        }
        for (int i = 0; i < 4; i++)
        {
            if (sphere_collision_SAT(b->location, b->size, mineParts[i], 4, 0))
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
                if (sphere_collision_SAT(p1->bullets[i].location, p1->bullets[i].size, smallParts[j], 3,p2->hideCollisionBox))
                {
                    p1->bullets[i].lifespan = 0;
                    return true;
                }
        }
    }
    return false;
}