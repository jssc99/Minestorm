#include <stdio.h>
#include <stdlib.h>
#include <math.h> //-lm to compile
#include "points.h"

const int DEBUG_POINTS = 0;
const float tol = 0.00001;
FILE *GEOGEBRA;
FILE *GEOGEBRA3;

// Set or Reset the coordinates of a Point2
Point2 setPoint2(float x, float y)
{
    Point2 temp = {x, y};
    if (DEBUG_POINTS == 1)
    {
        printf("New/Set/Reset Point2 = (%.2f , %.2f)\n", temp.x, temp.y);
    }
    return temp;
}
// Open the .txt for Geogebra
int geogebraScriptOpen(void)
{
    GEOGEBRA = fopen("toGeogebra.txt", "w+");
    fprintf(GEOGEBRA, "L={");

    return 0;
}

// Close the .txt for Geogebra
int geogebraScriptClose(char startPoint, char endPoint)
{
    fprintf(GEOGEBRA, "}\n\n");
    for (char c = startPoint; c <= endPoint; c++)
    {
        int i = c - startPoint + 1;
        fprintf(GEOGEBRA, "%c=L(%d)\n", c, i);
    }
    fclose(GEOGEBRA);
    return 0;
}

// Print in Geogebra the coodinates of a Point2 and if debug its name too in the console
void printPoint2(Point2 p, char str[])
{
    fprintf(GEOGEBRA, "%s(% .2f, % .2f)", (*str == 'A') ? "" : ",", p.x, p.y);
    if (DEBUG_POINTS >= 0)
        printf("Point2 %s = (%.2f,%.2f)\n", str, p.x, p.y);
}

// Create an opposite point of 'p'
Point2 oppPoint2(Point2 p)
{
    Point2 d = {-p.x, -p.y};
    if (DEBUG_POINTS == 1)
    {
        printf("New/Set/Reset Point2 = (%.2f , %.2f)\n", d.x, d.y);
    }
    return d;
}

// Create a point in the middle of '[p1,p2]'
Point2 midPoint2(Point2 p1, Point2 p2)
{
    Point2 p3 = {(p2.x + p1.x) / 2.f, (p2.y + p1.y) / 2.f};
    if (DEBUG_POINTS == 1)
    {
        printf("New/Set/Reset Point2 = (%.2f , %.2f)\n", p3.x, p3.y);
    }
    return p3;
}

// Rotate a point origin around 'center' from an angle
Point2 rotatePoint2(Point2 center, Point2 origin, float angle)
{
    Point2 dest;
    float ca = cosf(angle);
    float sa = sinf(angle);
    dest.x = (origin.x - center.x) * ca - (origin.y - center.y) * sa + center.x;
    dest.y = (origin.y - center.y) * ca + (origin.x - center.x) * sa + center.y;
    return dest;
}

// Measure of distance between 'p1' and 'p2'
float distPoint2(Point2 p1, Point2 p2)
{
    float distX = pow((p2.x - p1.x), 2);
    float distY = pow((p2.y - p1.y), 2);
    float dist = sqrt((distX) + (distY));

    if (DEBUG_POINTS == 1)
    {
        printf("Distance between two points = %.2f\n", dist);
    }
    return dist;
}

// Get the sides of a triangle with known points Side[1-3]
float *getTriangleSidePoints2(Point2 p1, Point2 p2, Point2 p3)
{
    float *side = malloc(sizeof(float) * 4);

    *(side + 1) = distPoint2(p2, p1);
    *(side + 2) = distPoint2(p3, p2);
    *(side + 3) = distPoint2(p1, p3);

    return side;
}

// Class sides from bigger to smaller sideInOrder[1-3] Not the smartest sort
float *getBiggerTriangleSide(Point2 p1, Point2 p2, Point2 p3)
{
    float *side = getTriangleSidePoints2(p1, p2, p3);
    float a, b, c;
    // Sort sides by size
    if (side[1] >= side[2])
    {
        if (side[2] >= side[3])
        {
            a = side[1];
            b = side[2];
            c = side[3];
        }
        else if (side[1] >= side[3])
        {
            a = side[1];
            b = side[3];
            c = side[2];
        }
        else if (side[1] >= side[2])
        {
            a = side[3];
            b = side[1];
            c = side[2];
        }
        else
        {
            a = side[3];
            b = side[2];
            c = side[1];
        }
    }
    else if (side[1] >= side[3])
    {
        a = side[2];
        b = side[1];
        c = side[3];
    }
    else
    {
        a = side[1];
        b = side[2];
        c = side[3];
    }
    float *sideInOrder = (float *)malloc(sizeof(float) * 4);
    sideInOrder[1] = a;
    sideInOrder[2] = b;
    sideInOrder[3] = c;
    return sideInOrder;
}

// Get the surface with Heron's formula
float getTriangleSurfacePoints2(Point2 p1, Point2 p2, Point2 p3)
{
    float *side = getBiggerTriangleSide(p1, p2, p3);
    float a, b, c;
    a = side[1];
    b = side[2];
    c = side[3];

    float heron;
    heron = (a + (b + c)) * (c - (a - b)) * (c + (a - b)) * (a + (b - c));
    float surface = (sqrt(heron) / 4);
    return surface;
}

// Returns 0 for flat 1 for any 2 for isosceles 3 for equilateral 4 for right 5 for isosceles right
int isTriangleSpecialPoints2(Point2 p1, Point2 p2, Point2 p3)
{
    int isSpecial = 0;
    float *side = getBiggerTriangleSide(p1, p2, p3);
    printf("\nThe triangle is ");
    float sqr1 = pow(side[1], 2);
    float sqr2 = pow(side[2], 2);
    float sqr3 = pow(side[3], 2);
    int pythagoras = (fabsf(sqr2 + sqr3 - sqr1) < tol);

    if (!side[1] || !side[2] || !side[3])
    {
        printf("flat");
    }
    else if (side[2] == side[3])
    {
        if (side[1] == side[3] && side[1])
        {
            isSpecial = 3;
            printf("equilateral");
        }
        else
        {
            printf("isosceles");
            isSpecial = 2;
            if (pythagoras)
            {
                isSpecial = 5;
                printf(" right");
            }
        }
    }
    else if (pythagoras)
    {
        printf("right");
        isSpecial = 4;
    }
    else
    {
        isSpecial = 1;
        printf("any");
    }
    printf(".\n");
    return isSpecial;
}

// Get the isobarycenter of the triangle
Point2 isobarycenterTriangle(Point2 p1, Point2 p2, Point2 p3)
{
    Point2 iso = {(p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3};
    return iso;
}

// Get the circumcenter of the triangle
Point2 circumcenterTriangle(Point2 p1, Point2 p2, Point2 p3)
{
    Point2 U;
    float d = 2.0 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
    U.x = ((pow(p1.x, 2.0) + pow(p1.y, 2.0)) * (p2.y - p3.y) + (pow(p2.x, 2.0) + pow(p2.y, 2.0)) * (p3.y - p1.y) + (pow(p3.x, 2.0) + pow(p3.y, 2.0)) * (p1.y - p2.y)) / d;
    U.y = ((pow(p1.x, 2.0) + pow(p1.y, 2.0)) * (p3.x - p2.x) + (pow(p2.x, 2.0) + pow(p2.y, 2.0)) * (p1.x - p3.x) + (pow(p3.x, 2.0) + pow(p3.y, 2.0)) * (p2.x - p1.x)) / d;
    return U;
}

// Get the perimeter of a circle
float perimeterCircle(Point2 a, Point2 b)
{
    float radius = distPoint2(a, b);
    return (2 * M_PI * radius);
}

// Get the surface of a circle
float surfaceCircle(Point2 a, Point2 b)
{
    float radius = distPoint2(a, b);
    return (M_PI * powf(radius, 2.0));
}

// Get the sides of a quadrilateral with known points Side[1-4]
float *getQuadrilateralSidePoints2(Point2 p1, Point2 p2, Point2 p3, Point2 p4)
{
    float *side = malloc(sizeof(float) * 5);

    *(side + 1) = distPoint2(p2, p1);
    *(side + 2) = distPoint2(p3, p2);
    *(side + 3) = distPoint2(p4, p3);
    *(side + 3) = distPoint2(p1, p4);

    return side;
}

// Get the surface with Brahmagupta's formula
float getQuadrilateralSurfacePoints2(Point2 p1, Point2 p2, Point2 p3, Point2 p4)
{

    float *side = getQuadrilateralSidePoints2(p1, p2, p3, p4);
    float a, b, c, d;
    a = side[1];
    b = side[2];
    c = side[3];
    d = side[4];

    float hp = (a + b + c + d) / 2;
    float heron = (hp - a) * (hp - b) * (hp - c) * (hp - d);
    float surface = sqrt(heron);
    return surface;
}

// 3D

// Set or Reset the coordinates of a Point3
Point3 setPoint3(float x, float y, float z)
{
    Point3 temp = {x, y, z};

    if (DEBUG_POINTS == 1)
    {
        printf("New/Set/Reset Point3 = (%.2f , %.2f,%.2f)\n", temp.x, temp.y, temp.z);
    }
    return temp;
}

int geogebra3dScriptOpen(void)
{
    GEOGEBRA3 = fopen("toGeogebra3d.txt", "w+");
    fprintf(GEOGEBRA3, "L={");

    return 0;
}

int geogebra3dScriptClose(char startPoint, char endPoint)
{
    if (!startPoint)
        fclose(GEOGEBRA3);
    fprintf(GEOGEBRA3, "}\n\n");
    for (char c = startPoint; c <= endPoint; c++)
    {
        int i = c - startPoint + 1;
        fprintf(GEOGEBRA3, "%c=L(%d)\n", c, i);
    }
    fclose(GEOGEBRA3);
    return 0;
}

// Print in Geogebra the coodinates of a Point3 and if debug its name too in the console
void printPoint3(Point3 p, char str[])
{
    fprintf(GEOGEBRA3, "%s(% .2f, % .2f, %.2f)", (*str == 'A') ? "" : ",", p.x, p.y, p.z);
    if (DEBUG_POINTS >= 0)
        printf("Point3 %s = (%.2f,%.2f,%.2f)\n", str, p.x, p.y, p.z);
}

// Create an opposite point of 'p'
Point3 oppPoint3(Point3 p)
{
    Point3 d = {-p.x, -p.y, -p.z};
    if (DEBUG_POINTS == 1)
    {
        printf("New/Set/Reset Point3 = (%.2f , %.2f, %.2f)\n", d.x, d.y, d.z);
    }
    return d;
}
// Create a point in the middle of '[p1,p2]'
Point3 midPoint3(Point3 p1, Point3 p2)
{
    Point3 p3 = {(p2.x + p1.x) / 2, (p2.y + p1.y) / 2, (p2.z + p1.z) / 2};
    if (DEBUG_POINTS == 1)
    {
        printf("New/Set/Reset Point3 = (%.2f , %.2f, %.2f)\n", p3.x, p3.y, p3.z);
    }
    return p3;
}

// Measure of distance between 'p1' and 'p2'
float distPoint3(Point3 p1, Point3 p2)
{
    float distX = pow((p2.x - p1.x), 2);
    float distY = pow((p2.y - p1.y), 2);
    float distZ = pow((p2.z - p1.z), 2);
    float dist = sqrt(distX + distY + distZ);

    if (DEBUG_POINTS == 1)
    {
        printf("Distance between two points = %.2f\n", dist);
    }
    return dist;
}

// Get the sides of a triangle with known points Side[1-3]
float *getTriangleSidePoint3(Point3 p1, Point3 p2, Point3 p3)
{
    // Point3 points[3] = {p1, p2, p3};
    float *side = malloc(sizeof(float) * 4);

    *(side + 1) = distPoint3(p2, p1);
    *(side + 2) = distPoint3(p3, p2);
    *(side + 3) = distPoint3(p1, p3);

    return side;
}

// Class sides from bigger to smaller sideInOrder[1-3]
float *getBiggerTriangleSidePoint3(Point3 p1, Point3 p2, Point3 p3)
{
    float *side = getTriangleSidePoint3(p1, p2, p3);
    float a, b, c;
    // Sort sides by size
    if (side[1] >= side[2])
    {
        if (side[2] >= side[3])
        {
            a = side[1];
            b = side[2];
            c = side[3];
        }
        else if (side[1] >= side[3])
        {
            a = side[1];
            b = side[3];
            c = side[2];
        }
        else if (side[1] >= side[2])
        {
            a = side[3];
            b = side[1];
            c = side[2];
        }
        else
        {
            a = side[3];
            b = side[2];
            c = side[1];
        }
    }
    else if (side[1] >= side[3])
    {
        a = side[2];
        b = side[1];
        c = side[3];
    }
    else
    {
        a = side[1];
        b = side[2];
        c = side[3];
    }
    float *sideInOrder = (float *)malloc(sizeof(float) * 4);
    sideInOrder[1] = a;
    sideInOrder[2] = b;
    sideInOrder[3] = c;
    return sideInOrder;
}

// Get the surface with Heron formula
float getTriangleSurfacePoint3(Point3 p1, Point3 p2, Point3 p3)
{
    float *side = getBiggerTriangleSidePoint3(p1, p2, p3);
    float a, b, c;
    a = side[1];
    b = side[2];
    c = side[3];

    float heron;
    heron = (a + (b + c)) * (c - (a - b)) * (c + (a - b)) * (a + (b - c));
    float surface = (sqrt(heron) / 4);
    return surface;
}

// Returns 0 for flat 1 for any 2 for isosceles 3 for equilateral 4 for right 5 for isosceles right
int isTriangleSpecialPoints3(Point3 p1, Point3 p2, Point3 p3)
{

    int isSpecial = 0;
    float *side = getBiggerTriangleSidePoint3(p1, p2, p3);
    float sqr1 = pow(side[1] * 10000, 2);
    float sqr2 = pow(side[2] * 10000, 2);
    float sqr3 = pow(side[3] * 10000, 2);
    printf("\nThe triangle is ");
    int pythagoras = (fabsf(sqr2 + sqr3 - sqr1) < tol);
    if (!side[1] || !side[2] || !side[3])
    {
        printf("flat");
    }
    else if (side[2] == side[3])
    {
        if (side[1] == side[3] && side[1])
        {
            isSpecial = 3;
            printf("equilateral");
        }
        else
        {
            printf("isosceles");
            isSpecial = 2;
            if (pythagoras)
            {
                isSpecial = 5;
                printf(" right");
            }
        }
    }
    else if (pythagoras)
    {
        printf("right");
        isSpecial = 4;
    }
    else
    {
        isSpecial = 1;
        printf("any");
    }
    printf(".\n");
    return isSpecial;
}

// Get the isobarycenter of the triangle
Point3 isobarycenterTrianglePoint3(Point3 p1, Point3 p2, Point3 p3)
{
    Point3 iso = {(p1.x + p2.x + p3.x) / 3,
                  (p1.y + p2.y + p3.y) / 3,
                  (p1.z + p2.z + p3.z) / 3};
    return iso;
}

// Work in progress

// Rotate a point origin around 'center' from an angle theta vertical and phi horizontal
Point3 rotatePoint3(Point3 center, Point3 origin, float theta, float phi)
{
    Point3 dest;
    float ct = cosf(theta);
    float st = sinf(theta);
    float sp = sinf(phi);
    float dist = distPoint3(center, origin);
    dest.x = dist * st - sp + center.x;
    dest.y = dist * st * sp + center.y;
    dest.z = dist * ct + center.z;
    return dest;
}
// Rotate a point origin around 'center' from an angle phi horizontal
Point3 rotateHorPoint3(Point3 center, Point3 origin, float phi)
{
    Point3 dest = origin;
    float c = cosf(phi);
    float s = sinf(phi);
    dest.x = (origin.x - center.x) * c - (origin.y - center.y) * s + center.x;
    dest.y = (origin.y - center.y) * c + (origin.x - center.x) * s + center.y;
    return dest;
}
// Rotate a point origin around 'center' from an angle theta vertical
Point3 rotateVerPoint3(Point3 center, Point3 origin, float theta)
{
    Point3 dest = origin;
    float c = cosf(theta);
    float dist = fabsf(origin.z - center.z);
    dest.z = dist * c + center.z;
    return dest;
}