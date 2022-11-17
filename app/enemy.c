#include "enemy.h"
#include "physics.h"

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
    enemy.type = type;
    enemy.angle = 0.f;

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
    return enemy;
}

void update_pos_basic_mine(Enemy *e, Vector2 pPos, bool alignPoints)
{
    if (e->type == MAGNETIC || e->type == MAGNET_FIRE)
    {   // normalise vector from enemy to player
        e->location.x = normalizedVector2((Vector2){pPos.x - e->location.origin.x, pPos.y - e->location.origin.y});
        e->location.y = rotatePoint2((Point2){0, 0}, e->location.x, M_PI / 2.f);
        e->angle = getAngleVector2((Float2){-1, 0}, e->location.x);
    }

    cvAddLine(e->location.origin.x, e->location.origin.y, e->location.origin.x + 20 * e->location.x.x, e->location.origin.y + 20 * e->location.x.y, CV_COL32(255, 0, 0, 255));
    cvAddLine(e->location.origin.x, e->location.origin.y, e->location.origin.x + 20 * e->location.y.x, e->location.origin.y + 20 * e->location.y.y, CV_COL32(0, 255, 0, 255));

    float radiusBig = get_max_size(e->size, e->type);
    float radiusSmall = get_small_size(e->size, e->type);

    e->location.origin.x += e->location.x.x;
    e->location.origin.y += e->location.x.y;

    poly_collision_border_replace(e->points, &e->location.origin, e->nbPoints, radiusBig, (Point2){700, 800});

    Point2 center = e->location.origin;
    float angle = e->angle;
    float rotation = M_PI / ((float)e->nbPoints / 2.f);

    for (int i = 0; i < e->nbPoints; i += 2)
    {
        e->points[i] = rotatePoint2(center, (Point2){center.x, center.y + radiusBig}, angle);
        if (!alignPoints)
            angle += rotation;
        (e->points[i + 1]) = rotatePoint2(center, (Point2){center.x, center.y + radiusSmall}, angle);
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

    poly_collision_border_replace(e->points, &e->location.origin, e->nbPoints, MINELAYER_LENGTH_X, (Point2){700, 800});

    float x = e->location.origin.x;
    float y = e->location.origin.y;

    Point2 point[9] = {
        {x - 26.f, y + 10.f},
        {x - 38.f, y + 20.f},
        {x - 32., y},
        {x - 14.f, y},
        {x /*  */, y - 12.f},
        {x + 14.f, y},
        {x + 32., y},
        {x + 38.f, y + 20.f},
        {x + 26.f, y + 10.f}};

    for (int i = 0; i < 9; i++)
        e->points[i] = rotatePoint2(e->location.origin, point[i], e->angle);
}

void update_pos_any_mine(Enemy *e, Vector2 posPlayer)
{
    switch (e->type)
    {
    case FLOATING:
    case FIREBALL_MINE:
    case MAGNETIC:
        update_pos_basic_mine(e, posPlayer, 0);
        break;

    case MAGNET_FIRE:
        update_pos_basic_mine(e, posPlayer, 1);
        break;

    case MINELAYER:
        update_pos_minelayer(e);
        break;
    }
}

void create_minefield(Enemy e[], int nbEnemy, int width, int height)
{
    srand(time(NULL));
    for (int i = 0; i < nbEnemy; i++)
    {
        e[i].location.origin.x = 10 + rand() % (width - 20);
        e[i].location.origin.y = 160 + rand() % (height - 180);
        e[i].status = CHILD;
    } /*
     printf("[ ");
     for (int i = 0; i < nbEnemy; ++i)
         printf("%.2f,%.2f%s", e[i].location.origin.x, e[i].location.origin.y, (i < nbEnemy - 1) ? ", " : " ");
     printf("]\n");*/
}