#include "Tools/axis.h"

enum type_enemy
{
    FLOATING,
    FIREBALL_MINE,
    MAGNETIC,
    MAGNET_FIRE,
    FIREBALL,
    MINELAYER
};

enum size_enemy
{
    SMALL,
    MEDIUM,
    BIG
};

typedef struct Enemy
{
    Axis2 location;
    bool born;
    type_enemy type;
    size_enemy size;
    float speed;
} Enemy;

void drawEnemy(Enemy enemy);