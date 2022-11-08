#include "Tools/axis.h"

enum type_ennemy
{
    FLOATING,
    FIREBALL_MINE,
    MAGNETIC,
    MAGNET_FIRE,
    FIREBALL,
    MINELAYER
};

enum size_ennemy
{
    SMALL,
    MEDIUM,
    BIG
};

typedef struct ennemy
{
    Axis2 location;
    bool born;
    type_ennemy type;
    size_ennemy size;
    float speed;
} ennemy;

