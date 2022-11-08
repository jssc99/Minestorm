#include "Tools/axis.h"

typedef struct Player
{
    Axis2 axis;
    Vector2 targetLine;
    Vector2 moveLine;
    float speed;
    int lives;
}Player;
