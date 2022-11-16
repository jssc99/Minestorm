#include "physics.h"

#define DEBUG true
// Detect a collision between a sphere and a SAT
bool sphere_collision_SAT(Point2 center, float radius, Point2 poly[], int nbSegments)
{
    for (int i = 0; i < nbSegments; i++)
    {
        Sat range = generate_SAT(poly, i, nbSegments);
        float proj = dotProductVector2(center, range.normal);
        if (proj < range.min - radius || proj > range.max + radius)
        {
            if (DEBUG)
            {
                cvAddLine(poly[i].x, poly[i].y, poly[(i + 1) % nbSegments].x, poly[(i + 1) % nbSegments].y, CV_COL32(255 * (!i), 255 * (i != 1), 0, 255));
            }
            return false;
        }
    }
    return true;
}
// Detect a collision between 2 SAT (first one with the most sides)
bool SAT_collision_SAT(Point2 poly1[], int sidesP1, Point2 poly2[], int sidesP2)
{
    for (int i = 0; i < sidesP1; i++)
    {
        Sat range = generate_SAT(poly1, i, sidesP1);
        float proj = dotProductVector2(poly2[0], range.normal);
        float min2 = proj;
        float max2 = proj;
        int jSep = i;
        for (int j = 1; j < sidesP2; j++)
        {
            proj = dotProductVector2(poly2[j], range.normal);
            if (proj < min2)
            {
                min2 = proj;
                jSep = j;
            }
            if (proj > max2)
            {
                max2 = proj;
                jSep = j;
            }
        }
        if (max2 < range.min || min2 > range.max)
        {
            if (DEBUG)
            {
                cvAddLine(poly1[i].x, poly1[i].y, poly1[(i + 1) % sidesP1].x, poly1[(i + 1) % sidesP1].y, CV_COL32(255 * (!i), 255 * (i != 1), 0, 255));
                cvAddLine(poly2[jSep].x, poly2[jSep].y, poly2[(jSep + 1) % sidesP2].x, poly2[(jSep + 1) % sidesP2].y, CV_COL32(255, 255, 0, 255));
            }
            return false;
        }
    }
    return true;
}

// type to determinate
Sat generate_SAT(Point2 p[], int pIdx, int nbSegments)
{
    Sat range;
    float dot;
    Vector2 v1 = setVector2Point2(p[pIdx], p[(pIdx + 1) % nbSegments]);
    range.normal = getNormalVectorVector2(v1);
    dot = dotProductVector2(p[0], range.normal);
    range.min = dot;
    range.max = dot;
    for (int i = 1; i < nbSegments; i++)
    {
        dot = dotProductVector2(p[i], range.normal);
        if (dot < range.min)
            range.min = dot;
        if (dot > range.max)
            range.max = dot;
    }
    return range;
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
    if (p.x > xmax || p.x < xmin || p.y > ymax || p.y < ymin)
        return false;
    return true;
}
// Checks collision between a sphere/point and a rectangle
// (if radius = 1.f -> sphere becomes point)
bool sphere_collision_rectangle(Point2 p, float radius, float xmin, float ymin, float xmax, float ymax)
{
    return point_collision_rectangle(p, xmin + radius, ymin + radius, xmax - radius, ymax - radius);
}
