#include "Tools/axis.h"

typedef enum enemyType
{
    FLOATING,
    FIREBALL_MINE,
    MAGNETIC,
    MAGNET_FIRE,
    FIREBALL,
    MINELAYER
} enemyType;

typedef enum enemySize
{
    SMALL,
    MEDIUM,
    BIG
} enemySize;

typedef struct Enemy
{
    Axis2 location;
    _Bool isBorn;
    float speed;
    enemyType type;
    enemySize size;
} Enemy;

int draw_enemy(Enemy *enemy);
Enemy *init_enemy(float x, float y, enemySize size, enemyType type);
