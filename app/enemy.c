#include "enemy.h"
#include "physics.h"

// returns a mines size multiplier
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

// returns a mines biggest radius
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

// returns a mines smallest inside radius
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

// ages an enemy, loops around (DEAD -> BABY)
void age_enemy(Enemy *e)
{
    e->status = (e->status + 1) % 4;
}

// changes an enemy 'size', unless size is FIXED, loops around (BIG -> SMALL)
void size_enemy(Enemy *e)
{
    if (e->size != FIXED)
        e->size = (e->size + 1) % 3;
}

// places enemy at a chosen point
void set_pos_enemy(Enemy *e, float x, float y)
{
    e->location.origin = (Point2){x, y};
}

// init enemy type and size, requires pos of player
Enemy init_enemy(Vector2 pPos, enemyType type, enemySize size)
{
    Enemy enemy;
    enemy.status = BABY;
    enemy.type = type;
    if (type == FLOATING || type == FIREBALL_MINE)
    {
        enemy.angle = (rand() % 360) * M_PI / 180.f;
        enemy.location.origin = (Point2){100, 100};
        enemy.location.x = normalizedVector2(rotatePoint2((Point2){1.f, 0.f}, enemy.location.origin, enemy.angle));
        enemy.location.y = rotatePoint2((Point2){0, 0}, enemy.location.x, M_PI / 2.f);
    }
    else
    {
        enemy.angle = 0.f;
        enemy.location = (Axis2){{100, 100}, {1.f, 0.f}, {0.f, 1.f}};
    }

    if (size == FIXED && type != MINELAYER && type != FIREBALL)
        size = SMALL; // J'ai retire un = en trop, Warning

    switch (type)
    {
    case FLOATING:
        enemy.nbPoints = 6;
        if (size == BIG)
            enemy.deathScore = 100;
        if (size == MEDIUM)
            enemy.deathScore = 135;
        if (size == SMALL)
            enemy.deathScore = 200;
        break;

    case FIREBALL_MINE:
        enemy.nbPoints = 8;
        if (size == BIG)
            enemy.deathScore = 325;
        if (size == MEDIUM)
            enemy.deathScore = 360;
        if (size == SMALL)
            enemy.deathScore = 425;
        break;

    case MAGNETIC:
        enemy.nbPoints = 8;
        if (size == BIG)
            enemy.deathScore = 500;
        if (size == MEDIUM)
            enemy.deathScore = 535;
        if (size == SMALL)
            enemy.deathScore = 600;
        break;

    case MAGNET_FIRE:
        enemy.nbPoints = 8;
        if (size == BIG)
            enemy.deathScore = 750;
        if (size == MEDIUM)
            enemy.deathScore = 785;
        if (size == SMALL)
            enemy.deathScore = 850;
        break;

    case FIREBALL:
        enemy.nbPoints = 0;
        enemy.deathScore = 110;
        enemy.size = FIXED;
        break;

    case MINELAYER:
        enemy.nbPoints = 9;
        enemy.deathScore = 1000;
        enemy.size = FIXED;
        break;

    default:
        enemy.nbPoints = 0;
        enemy.deathScore = 0;
        enemy.size = size;
        break;
    }
    return enemy;
}

void update_pos_basic_mine(Enemy *e, Vector2 pPos, bool alignPoints)
{
    if (e->type == MAGNETIC || e->type == MAGNET_FIRE)
    { // normalise vector ( enemy to player )
        e->location.x = normalizedVector2((Vector2){pPos.x - e->location.origin.x, pPos.y - e->location.origin.y});
        if (700 - fabsf(e->location.origin.x - pPos.x) <= fabsf(e->location.origin.x - pPos.x))
            e->location.x.x *= -1;
        if (800 - fabsf(e->location.origin.y - pPos.y) <= fabsf(e->location.origin.y - pPos.y))
            e->location.x.y *= -1;
        e->location.y = rotatePoint2((Point2){0, 0}, e->location.x, M_PI / 2.f);
        e->angle = getAngleVector2((Float2){-1, 0}, e->location.x);
    }
    e->location.origin.x += e->location.x.x;
    e->location.origin.y += e->location.x.y;

    float radiusBig = get_max_size(e->size, e->type);
    float radiusSmall = get_small_size(e->size, e->type);

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

void update_pos_fireball(Enemy *e, Vector2 pPos)
{
    if (!e->nbPoints) // === bool fireball nbPoints updated ('points' for fireball is unused)
    {
        e->location.x = normalizedVector2((Vector2){pPos.x - e->location.origin.x, pPos.y - e->location.origin.y});
        e->location.y = rotatePoint2((Point2){0, 0}, e->location.x, M_PI / 2.f);
        e->nbPoints++;
    }
    e->location.origin.x += e->location.x.x;
    e->location.origin.y += e->location.x.y;

    float radiusBig = get_max_size(e->size, e->type);

    if (sphere_collision_rectangle(e->location.origin, radiusBig, 0, 0, 700, 800))
        e->status = DEAD;
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
        {x - 32.f, y},
        {x - 14.f, y},
        {x /*  */, y - 12.f},
        {x + 14.f, y},
        {x + 32.f, y},
        {x + 38.f, y + 20.f},
        {x + 26.f, y + 10.f}};

    for (int i = 0; i < 9; i++)
        e->points[i] = rotatePoint2(e->location.origin, point[i], e->angle);
}

// updates the pos of any mine inputed
void update_pos_any_enemy(Enemy *e, Vector2 posPlayer)
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

    case FIREBALL:
        update_pos_fireball(e, posPlayer);
        break;

    case MINELAYER:
        update_pos_minelayer(e);
        break;
    }
}

// creates the minefield by giving every enemy a pos
void create_minefield(Enemy e[], int nbEnemy, int width, int height)
{
    srand(time(NULL));
    for (int i = 0; i < nbEnemy; i++)
    {
        e[i].location.origin.x = 100 + rand() % (width - 120);
        e[i].location.origin.y = 160 + rand() % (height - 180);
        e[i].status = CHILD;
    }
}