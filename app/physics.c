#include "physics.h"

// SAT

bool sphere_collision_SAT(Point2 center, float radius, Sat sat[], int nbSegments)
{
    for (int i = 0; i < nbSegments; i++)
    {
        float proj = dotProductVector2(center,sat[i].normal);
        if (proj > sat[i].min - radius || proj < sat->max + radius)
            return false;
    }
    return true;
}
// type to determinate
Sat* generate_SAT(Point2 p[], int nbSegments)
{
    Sat* range = igMemAlloc(nbSegments * sizeof(Sat));
    for (int i = 0; i < nbSegments; i++)
    {
        Vector2 v1 = setVector2Point2(p[i], p[(i + 1) % nbSegments]);
        range[i].normal = getNormalVectorVector2(v1);
        range[i].max = dotProductVector2(p[i], range[i].normal);
        range[i].min = dotProductVector2(p[(i + 2) % nbSegments], range[i].normal);
    }
    return range;
}

// Checks collision between 2 spheres
bool SS_collision_SS(Point2 p1, float size1, Point2 p2, float size2)
{
    if (distPoint2(p1, p2) <= size1 + size2)
        return true;
    return false;
}
// Checks collision between a point and a rectangle
bool point_collision_rectangle(Point2 p, float xmin, float ymin, float xmax, float ymax)
{
    if (p.x <= xmax && p.x >= xmin && p.y <= ymax && p.y >= ymin)
        return false;
    return true;
}
// Checks collision between a sphere and a rectangle
bool SS_collision_rectangle(Point2 p, float size, float xmin, float ymin, float xmax, float ymax)
{
    return point_collision_rectangle(p, xmin + size, ymin + size, xmax - size, ymax - size);
}