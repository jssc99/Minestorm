#include "physics.h"

bool SS_collision_SS(Point2 p1, float size1, Point2 p2, float size2)
{
    if (distPoint2(p1,p2) <= size1+size2)
        return true;
    return false;
}
// Checks collision between a point and a rectangle
bool point_collision_rectangle(Point2 p, float xmin, float ymin, float xmax, float ymax)
{
    if (p.x <= xmax  && p.x >= xmin && p.y <= ymax && p.y >= ymin)
        return false;
    return true;
}
// Checks collision between a sphere and a rectangle
bool SS_collision_rectangle(Point2 p, float size, float xmin, float ymin, float xmax, float ymax)
{
    return point_collision_rectangle(p, xmin+size,ymin + size, xmax-size, ymax-size);
    //if (p.x <= xmax - size && p.x >= xmin + size && p.y <= ymax - size && p.y >= ymin + size)
    //    return false;
    //return true;
}