#include "axis.h"

// Move the local axis in 2D following vector v
Axis2 translateAxis2(Axis2 a, Vector2 v)
{
    Axis2 ta = a;
    ta.origin = translatePoint2(a.origin, v);
    return ta;
}

// Scale the local axis in 2D by 'ratio'
Axis2 scaleAxis2(Axis2 a, float ratio)
{
    Axis2 sa = {a.origin,
                multVector2(a.x, ratio),  // x
                multVector2(a.y, ratio)}; // y
    return sa;
}

// Rotate the local axis in 2D by 'angle'
Axis2 rotateAxis2(Axis2 a, float angle)
{
    Axis2 ra = {a.origin,
                rotatePoint2((Float2){0,0}, a.x, angle),  // x
                rotatePoint2((Float2){0,0}, a.y, angle)}; // y
    return ra;
}