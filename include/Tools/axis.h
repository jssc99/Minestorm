#pragma once

#include "matrix.h"

typedef struct Axis2
{
    Point2 origin;
    Vector2 x; // could use a matrix
    Vector2 y;
} Axis2;

Axis2 translateAxis2(Axis2 a, Vector2 v);
Axis2 scaleAxis2(Axis2 a, float ratio);
Axis2 rotateAxis2(Axis2 a, float angle);