#include "physics.h"

#define DRAW_LIST igGetBackgroundDrawList_Nil()
//SAT

// Detect a collision between 2 SAT (first one with the most non parallel sides)
bool SAT_collision_SAT(Point2 poly1[], int sidesP1, Point2 poly2[], int sidesP2, bool displaySAT)
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
            if (displaySAT)
            {
                ImDrawList_AddLine(DRAW_LIST, (ImVec2){poly1[i].x, poly1[i].y}, (ImVec2){poly1[(i + 1) % sidesP1].x, poly1[(i + 1) % sidesP1].y}, CV_COL32(255, 200 + 55 * (!i), 0, 200), 2.0f);
                ImDrawList_AddLine(DRAW_LIST, (ImVec2){poly2[jSep].x, poly2[jSep].y}, (ImVec2){poly2[(jSep + 1) % sidesP2].x, poly2[(jSep + 1) % sidesP2].y}, CV_COL32(255, 200 + 55 * (!jSep), 0, 200), 2.0f);
            }
            return false;
        }
    }
    return true;
}

// Detect a collision between a sphere and a SAT
bool sphere_collision_SAT(Point2 center, float radius, Point2 poly[], int nbSegments, bool displaySAT)
{
    for (int i = 0; i < nbSegments; i++)
    {
        Sat range = generate_SAT(poly, i, nbSegments);
        float proj = dotProductVector2(center, range.normal);
        if (proj < range.min - radius || proj > range.max + radius)
        {
            if (displaySAT)
            {
                ImDrawList_AddLine(DRAW_LIST, (ImVec2){poly[i].x, poly[i].y}, (ImVec2){poly[(i + 1) % nbSegments].x, poly[(i + 1) % nbSegments].y}, CV_COL32(255, 200 + 55 * (!i), 0, 200), 2.0f);
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
    range.min = dot;        //Initialize min and max
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

//Spheres

// Check collision between 2 spheres/points
// (if radius = 0.f -> sphere becomes point)
bool sphere_collision_sphere(Point2 p1, float radius1, Point2 p2, float radius2)
{
    return (distPoint2(p1, p2) <= radius1 + radius2);
}

// Check collision between a point and a rectangle
bool point_collision_rectangle(Point2 p, float xmin, float ymin, float xmax, float ymax)
{
    return (p.x > xmax || p.x < xmin || p.y > ymax || p.y < ymin);
}

// Check collision between a sphere/point and a rectangle
// (if radius = 0.f -> sphere becomes point)
bool sphere_collision_rectangle(Point2 p, float radius, float xmin, float ymin, float xmax, float ymax)
{
    return point_collision_rectangle(p, xmin + radius, ymin + radius, xmax - radius, ymax - radius);
}

// Check collision between a sphere and the screen and replace the object
void sphere_collision_border_replace(Point2 *p, float size, Point2 maxScreen)
{
    if (p->x > maxScreen.x - size)
        p->x = size;
    else if (p->x < size)
        p->x = maxScreen.x - size;
    if (p->y > maxScreen.y - size)
        p->y = size;
    else if (p->y < size)
        p->y = maxScreen.y - size;
}

//Poly

// Check collision between a polygon and a border
// 1-Check the circumcirlce first
// 2-Then for each summit 'p' of a polygon, check if outside the screen.
void poly_collision_border_replace(Point2 p[], Point2 *center, int nbSides, float size, Point2 maxScreen)
{
    if (sphere_collision_rectangle(*center, size, 0, 0, maxScreen.x, maxScreen.y))
    {
        for (int i = 0; i < nbSides; i++)
        {
            if (p[i].x > maxScreen.x)
                center->x = size;
            else if (p[i].x < 0)
                center->x = maxScreen.x - size;
            if (p[i].y > maxScreen.y)
                center->y = size;
            else if (p[i].y < 0)
                center->y = maxScreen.y - size;
        }
    }
}

//Add an OBB for the mines