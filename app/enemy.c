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
        return 11.7 * multiplier;
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

// checks in array of enemy if one is not dead
bool is_any_enemy_alive(Enemy e[], int nbEnemies)
{
    for (int i = 0; i < nbEnemies; i++)
        if (e[i].status != DEAD)
            return 1;
    return 0;
}

// checks in array of enemy if one is not dead
int how_many_e_adult(Enemy e[], int nbEnemies)
{
    int cpt = 0;
    for (int i = 0; i < nbEnemies; i++)
        if (e[i].status == ADULT)
            cpt++;
    return cpt;
}

// checks in array of enemy if one is not dead
int how_many_e_child(Enemy e[], int nbEnemies)
{
    int cpt = 0;
    for (int i = 0; i < nbEnemies; i++)
        if (e[i].status == CHILD)
            cpt++;
    return cpt;
}

// ages an enemy, loops around
// (DEAD -> BABY -> CHILD -> ADULT -> ...)
void age_enemy(Enemy *e)
{
    e->status = (e->status + 1) % 4;
}

// changes an enemy 'size', unless size is FIXED, loops around
//(BIG -> SMALL -> MEDIUM -> ...)
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

// init enemy type and size, point of origin (just use e.location.origin for most cases)
// gives them 'CHILD' status
Enemy init_enemy(Point2 origin, enemyType type, enemySize size)
{
    Enemy e;
    e.status = CHILD;
    e.type = type;
    if (type == FLOATING || type == FIREBALL_MINE)
    {
        e.angle = (rand() % 180) / M_PI;
        e.location.origin = origin;
        e.location.x = normalizedVector2(rotatePoint2((Point2){1.f, 0.f}, e.location.origin, e.angle));
        e.location.y = rotatePoint2((Point2){0, 0}, e.location.x, M_PI / 2.f);
    }
    else if (type == MINELAYER)
    {
        e.location.origin = (Point2){350, 50};
        e.location.x = (Vector2){0.f, 1.f};
        e.location.y = (Vector2){-1.f, 0.f};
        e.angle = 0.f;
    }
    else
    {
        e.angle = 0.f;
        e.location = (Axis2){origin, {1.f, 0.f}, {0.f, 1.f}};
    }

    if (size == FIXED && type != MINELAYER && type != FIREBALL)
        size = SMALL;
    if (size != FIXED && (type == MINELAYER || type == FIREBALL))
        size = FIXED;

    switch (type)
    {
    case FLOATING:
        e.nbPoints = 6;
        e.size = size;
        if (size == BIG)
            e.deathScore = 100;
        if (size == MEDIUM)
            e.deathScore = 135;
        if (size == SMALL)
            e.deathScore = 200;
        break;

    case FIREBALL_MINE:
        e.nbPoints = 8;
        e.size = size;
        if (size == BIG)
            e.deathScore = 325;
        if (size == MEDIUM)
            e.deathScore = 360;
        if (size == SMALL)
            e.deathScore = 425;
        break;

    case MAGNETIC:
        e.nbPoints = 8;
        e.size = size;
        if (size == BIG)
            e.deathScore = 500;
        if (size == MEDIUM)
            e.deathScore = 535;
        if (size == SMALL)
            e.deathScore = 600;
        break;

    case MAGNET_FIRE:
        e.nbPoints = 8;
        e.size = size;
        if (size == BIG)
            e.deathScore = 750;
        if (size == MEDIUM)
            e.deathScore = 785;
        if (size == SMALL)
            e.deathScore = 850;
        break;

    case FIREBALL:
        e.nbPoints = 0;
        e.deathScore = 110;
        e.size = FIXED;
        break;

    case MINELAYER:
        e.nbPoints = 9;
        e.deathScore = 1000;
        e.size = FIXED;
        break;

    default:
        e.nbPoints = 0;
        e.deathScore = 0;
        e.size = size;
        break;
    }
    return e;
}

// updates non-minelayer/fireball mines (pos, points of shape, rotation, magnetics track ppos, colision with border)
void update_pos_basic_mine(Enemy *e, bool alignPoints, Vector2 pPos)
{
    if (e->type == MAGNETIC || e->type == MAGNET_FIRE)
    { // normalise vector (enemy to player)
        e->location.x = normalizedVector2((Vector2){pPos.x - e->location.origin.x, pPos.y - e->location.origin.y});
        if (700 - fabsf(e->location.origin.x - pPos.x) <= fabsf(e->location.origin.x - pPos.x))
            e->location.x.x *= -1;
        if (800 - fabsf(e->location.origin.y - pPos.y) <= fabsf(e->location.origin.y - pPos.y))
            e->location.x.y *= -1;
        e->location.y = rotatePoint2((Point2){0, 0}, e->location.x, M_PI / 2.f);
        e->angle = getAngleVector2((Float2){-1, 0}, e->location.x);
    }
    e->location.origin.x += ((6.5f - get_size_multiplier(e->size)) / 2.f) * e->location.x.x;
    e->location.origin.y += ((6.5f - get_size_multiplier(e->size)) / 2.f) * e->location.x.y;

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

// updates the fireball (pos, targets ppos first use, collision with border (dies))
void update_pos_fireball(Enemy *e, Vector2 pPos)
{
    if (!e->nbPoints) // === bool, fireball nbPoints updated ('nbPoints' for fireball is unused)
    {
        e->location.x = normalizedVector2((Vector2){pPos.x - e->location.origin.x, pPos.y - e->location.origin.y});
        e->location.y = rotatePoint2((Point2){0, 0}, e->location.x, M_PI / 2.f);
        e->nbPoints++;
    }
    e->location.origin.x += 2.f * e->location.x.x;
    e->location.origin.y += 2.f * e->location.x.y;

    float radiusBig = get_max_size(e->size, e->type);

    if (sphere_collision_rectangle(e->location.origin, radiusBig, 0, 0, 700, 800))
        e->status = DEAD;
}

// updates the minelayer (pos, points of shape, rotation is possible, collision with border)
void update_pos_minelayer(Enemy *e)
{
    e->location.origin.x += e->location.x.x;
    e->location.origin.y += e->location.x.y;

    poly_collision_border_replace(e->points, &e->location.origin, e->nbPoints, MINELAYER_LENGTH_X, (Point2){700, 800});

    float x = e->location.origin.x;
    float y = e->location.origin.y;

    Point2 point[9] = {
        {x /*  */, y - 12.f},
        {x - 14.f, y},
        {x - 32.f, y},
        {x - 38.f, y + 20.f},
        {x - 26.f, y + 10.f},
        {x + 26.f, y + 10.f},
        {x + 38.f, y + 20.f},
        {x + 32.f, y},
        {x + 14.f, y}};

    for (int i = 0; i < 9; i++)
        e->points[i] = rotatePoint2(e->location.origin, point[i], e->angle);
}

// updates the pos of any SINGLE mine inputed
void update_pos_any_enemy(Enemy *e, Vector2 posPlayer)
{
    switch (e->type)
    {
    case FLOATING:
    case FIREBALL_MINE:
    case MAGNETIC:
        update_pos_basic_mine(e, 0, posPlayer);
        break;

    case MAGNET_FIRE:
        update_pos_basic_mine(e, 1, posPlayer);
        break;

    case FIREBALL:
        update_pos_fireball(e, posPlayer);
        break;

    case MINELAYER:
        update_pos_minelayer(e);
        break;
    }
}

// updates the pos of ALL (array) mines inputed
// calls update_pos_any_enemy()
void update_pos_all_enemy(Enemy e[], int size, Vector2 posPlayer)
{
    for (int i = 0; i < size; i++)
        if (e[i].status == ADULT)
            update_pos_any_enemy(&(e[i]), posPlayer);
}

// creates mines when minelayer activated
void minelayer_spawner(Enemy *e, Enemy en[], Point2 pPos)
{
    update_pos_any_enemy(e, pPos);
    bool found = 0;
    if ((int)e->location.origin.y == 200 ||
        (int)e->location.origin.y == 400 ||
        (int)e->location.origin.y == 600)
        for (int i = 0; i < MAX_ENEMY - 1; i++)
            if (!found && en[i].status == DEAD)
            {
                en[i] = init_enemy(e->location.origin, FLOATING, SMALL);
                en[i].status = ADULT;
                found = true;
            }
}

// creates the minefield by giving every enemy a random pos
// gives them 'BABY' status
void create_minefield(Enemy e[], int nbEnemy, int width, int height)
{
    srand(time(NULL));
    for (int i = 0; i < nbEnemy; i++)
    {
        do
        {
            e[i].location.origin.x = 100 + rand() % (width - 120);
            e[i].location.origin.y = 160 + rand() % (height - 200);
        } while (((e[i].location.origin.x > 230 && e[i].location.origin.x < 270) ||
                  (e[i].location.origin.x > 330 && e[i].location.origin.x < 370) ||
                  (e[i].location.origin.x > 430 && e[i].location.origin.x < 470)) &&
                 (e[i].location.origin.y > 370 && e[i].location.origin.y < 425)); // mines can't spwan near player possible pos
        e[i].status = BABY;
    }
}
