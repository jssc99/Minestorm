#include "physics.h"

bool sphere_collision_SAT(Point2 center, float radius, Sat *range, int nbSegments)
{
    for (int i = 0; i < nbSegments; i++)
    {
        // Vector2 v1 = setVector2Point2(p[i], p[(i + 1) % nbSegments]);
        // range[i].normal = getNormalVectorVector2(v1);
        // range[i].max = dotProductVector2(p[i], range[i].normal);
        // range[i].min = dotProductVector2(p[(i + 2) % nbSegments], range[i].normal);
        float proj = dotProductVector2(center, range[i].normal);
        if (proj > range[i].min - radius || proj < range[i].max + radius)
            return false;
    }
    return true;
}

// type to determinate
void generate_SAT(Sat *range, Point2 *p, int nbSegments)
{
    // Sat* range = igMemAlloc(nbSegments * sizeof(Sat));
    for (int i = 0; i < nbSegments; i++)
    {
        printf("(%f, %f)\n", p[i].x, p[i].y);
        Vector2 v1 = setVector2Point2(p[i], p[(i + 1) % nbSegments]);
        range[i].normal = getNormalVectorVector2(v1);
        range[i].max = dotProductVector2(p[i], range[i].normal);
        range[i].min = dotProductVector2(p[(i + 2) % nbSegments], range[i].normal);
        printf("min = %f, max = %f, x = %f, y = %f\n", range[i].min, range[i].max, range[i].normal.x, range[i].normal.y);
    }
    printf("\n");
}

// Checks collision between 2 spheres/points
// (if radius = 1.f -> sphere becomes point)
bool sphere_collision_sphere(Point2 p1, float radius1, Point2 p2, float radius2)
{
    if (distPoint2(p1, p2) <= radius1 + radius2)
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
// Checks collision between a sphere/point and a rectangle
// (if radius = 1.f -> sphere becomes point)
bool sphere_collision_rectangle(Point2 p, float radius, float xmin, float ymin, float xmax, float ymax)
{
    return point_collision_rectangle(p, xmin + radius, ymin + radius, xmax - radius, ymax - radius);
}