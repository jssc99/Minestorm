#include "player.h"

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

// Reset the bullets (*p2 = NULL if 1p game)
void bullets_terminate(Player *p1, Player *p2)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        p1->bullets[i].lifespan = 0;
        if (p2)
            p2->bullets[i].lifespan = 0;
    }
}
