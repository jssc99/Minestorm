#include "enemy.h"

float get_size_multiplier(enemySize size)
{
    switch (size)
    {
    case SMALL:
        return 2.f;
    case MEDIUM:
        return 4.f;
    case BIG:
    case FIXED:
        return 6.f;

    default:
        return 0.f;
    }
}

float get_max_size(enemySize size, enemyType type)
{
    float multiplier = get_size_multiplier(size);

    switch (type)
    {
    case FLOATING:
    case FIREBALL_MINE:
    case MAGNETIC:
    case MAGNET_FIRE:
        return MINE_BIG_RADIUS * multiplier;

    case FIREBALL:
        return multiplier;

    case MINELAYER:
        return 11.7 * multiplier; // 11.7 = distance origin to furthest point from origin of figure
    }
    return 0.f;
}

float get_small_size(enemySize size, enemyType type)
{
    float multiplier = get_size_multiplier(size);

    switch (type)
    {
    case FLOATING:
        return FLOATING_SMALL_RADIUS * multiplier;

    case FIREBALL_MINE:
        return FIREBALL_MINE_SMALL_RADIUS * multiplier;

    case MAGNETIC:
        return MAGNETIC_SMALL_RADIUS * multiplier;

    case MAGNET_FIRE:
        return MAGNET_FIRE_SMALL_RADIUS * multiplier;

    case FIREBALL:
        return multiplier;

    case MINELAYER:
        return 11.7 * multiplier; // 11.7 = distance origin to furthest point from origin of figure
    }
    return 0.f;
}

Enemy init_enemy(float x, float y, enemyType type, enemySize size)
{
    Enemy enemy;
    enemy.location = (Axis2){{x, y}, {1.f, 0.f}, {0.f, 1.f}};
    enemy.status = BABY;
    enemy.speed = 0.f;
    enemy.type = type;

    switch (type)
    {
    case MINELAYER:
    case FIREBALL:
        enemy.size = FIXED;
        break;

    default:
        enemy.size = size;
        break;
    }

    switch (type)
    {
    case FLOATING:
        enemy.nbPoints = 6;
        break;

    case FIREBALL_MINE:
    case MAGNETIC:
    case MAGNET_FIRE:
        enemy.nbPoints = 8;
        break;

    case MINELAYER:
        enemy.nbPoints = 9;
        break;

    case FIREBALL:
    default:
        enemy.nbPoints = 0;
        break;
    }

    if (type == FLOATING || type == FIREBALL_MINE)
    {
        srand(time(NULL));
        enemy.angle = ((rand() % 360) / M_PI);
    } // ADD ? IF TYPE = MAGNET, ANGLE = PLAYER POS
    else
        enemy.angle = 0.f;
    return enemy;
}

void update_pos_basic_mine(Enemy *e, bool alignPoints)
{
    float radius_big = get_max_size(e->size, e->type);
    float radius_small = get_small_size(e->size, e->type);
    e->location.origin.x += e->location.x.x;
    if (e->location.origin.x + radius_big >= 750)
        e->location.origin.x = radius_big;
    e->location.origin.y += e->location.x.y;
    if (e->location.origin.y + radius_big >= 800)
        e->location.origin.y = radius_big;
    Point2 center = e->location.origin;

    float angle = e->angle;
    float rotation = M_PI / ((float)e->nbPoints / 2.f);

    for (int i = 0; i < e->nbPoints; i += 2)
    {
        e->points[i] = rotatePoint2(center, (Point2){center.x, center.y + radius_big}, angle);
        if (!alignPoints)
            angle += rotation;
        (e->points[i + 1]) = rotatePoint2(center, (Point2){center.x, center.y + radius_small}, angle);
        if (alignPoints)
            angle += 2.f * rotation;
        else
            angle += rotation;
    }
}

void update_pos_minelayer(Enemy *e)
{
    e->location.origin.x += e->location.x.x;
    e->location.origin.y += e->location.x.y;
    e->location.origin.x += e->location.x.x;
    if (e->location.origin.x + 11.7 >= 750)
        e->location.origin.x = 11.7;
    e->location.origin.y += e->location.x.y;
    if (e->location.origin.y + MINE_BIG_RADIUS >= 800)
        e->location.origin.y = MINE_BIG_RADIUS;
    float x = e->location.origin.x;
    float y = e->location.origin.y;

    Point2 point[9] = {
        {x - 26.f, y + 10.f},
        {x - 36.f, y + 14.f},
        {x - 30.f, y},
        {x - 12.f, y},
        {x /*  */, y - 12.f},
        {x + 12.f, y},
        {x + 30.f, y},
        {x + 36.f, y + 14.f},
        {x + 26.f, y + 10.f}};

    for (int i = 0; i < 9; i++)
        e->points[i] = rotatePoint2(e->location.origin, point[i], e->angle);
}

void update_pos_any_mine(Enemy *e)
{
    if (e->type == FLOATING || e->type == FIREBALL_MINE || e->type == MAGNETIC)
        update_pos_basic_mine(e, 0);
    if (e->type == MAGNET_FIRE)
        update_pos_basic_mine(e, 1);
    // update_pos_magnet_fire_mine(e);
    if (e->type == MINELAYER)
        update_pos_minelayer(e);
}

void create_minefield(Enemy e[], int nbEnemy, int width, int height)
{
    srand(time(NULL));
    for (int i = 0; i < nbEnemy; i++)
    {
        e[i].location.origin.x = 10 + rand() % (width - 20);
        e[i].location.origin.y = 160 + rand() % (height - 180);
    } /*
     printf("[ ");
     for (int i = 0; i < nbEnemy; ++i)
         printf("%.2f,%.2f%s", e[i].location.origin.x, e[i].location.origin.y, (i < nbEnemy - 1) ? ", " : " ");
     printf("]\n");*/
}