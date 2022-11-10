#include <stdio.h>
#include <stdlib.h>
#include <math.h> //-lm to compile
#include "vectors.h"

const int DEBUG_VECTORS = 0;
FILE *GEOGEBRAV;
FILE *GEOGEBRA3V;

// Open toGeogebraV.txt
int geogebraVScriptOpen(void)
{
    GEOGEBRAV = fopen("toGeogebraV.txt", "w+");
    fprintf(GEOGEBRAV, "L={");

    return 0;
}

// Close toGeogebraV.txt
int geogebraVScriptClose(char startVector, char endVector)
{
    fprintf(GEOGEBRAV, "}\n\n");
    for (char c = startVector; c <= endVector; c++)
    {
        int i = c - startVector + 1;
        fprintf(GEOGEBRAV, "%c=L(%d)\n", c, i);
    }
    fclose(GEOGEBRAV);
    return 0;
}

// Print in Geogebra the coodinates of a Point2 and if debug its name too in the console
void printVector2(Vector2 v, char str[])
{
    fprintf(GEOGEBRAV, "%s(% .2f, % .2f)", (*str == 'a') ? "" : ",", v.x, v.y);
    if (DEBUG_VECTORS > 0)
        printf("Vector2 %s = (%.2f,%.2f)\n", str, v.x, v.y);
}

// Set Vector in 2D
Vector2 setVector2(float x, float y)
{
    Vector2 temp = {x, y};
    if (DEBUG_VECTORS == 1)
    {
        printf("New/Set/Reset Vector2 = (%.2f , %.2f)\n", temp.x, temp.y);
    }
    return temp;
}

// Set Vector in 2D fromm 2 points
Vector2 setVector2Point2(Point2 p1, Point2 p2)
{
    Vector2 temp = {p2.x - p1.x, p2.y - p1.y};
    if (DEBUG_VECTORS == 1)
    {
        printf("New/Set/Reset Vector2 = (%.2f , %.2f)\n", temp.x, temp.y);
    }
    return temp;
}

// Set the opposite of vector2d v
Vector2 oppVector2(Vector2 v)
{
    Vector2 a = setVector2(-v.x, -v.y);
    return a;
}

// Return the norm of the vector v 2D
float normVector2(Vector2 v)
{
    return (float)(sqrt(pow((v.x), 2.0) + pow((v.y), 2.0)));
}

// Add the 2 vectors in 2D
Vector2 addVector2(Vector2 v1, Vector2 v2)
{
    return (Float2){v1.x + v2.x, v1.y + v2.y};
}

// Multiplies a vector by a float
Vector2 multVector2(Vector2 v, float mul)
{
    return (Float2){v.x * mul, v.y * mul};
}

//Normalize Vector2
Vector2 normalizedVector2(Vector2 v)
{
    return multVector2(v, normVector2(v));
}

//Get the normal vector from a vector2
Vector2 getNormalVectorVector2(Vector2 v)
{
    return rotatePoint2((Float2){0,0}, normalizedVector2(v), M_PI);
}

//Get the normal vector from 2 points
Vector2 getNormalVectorPoint2(Point2 a, Point2 b)
{
    return getNormalVectorVector2(setVector2Point2(a,b));
}

// Dot product of 2 vectors in 2D
float dotProductVector2(Vector2 v1, Vector2 v2)
{
    return (float)(v1.x * v2.x + v1.y * v2.y);
}

// Determinant product of 2 vectors in 2D
float determinantVector2(Vector2 v1, Vector2 v2)
{
    return (float)(v1.x * v2.y) - (v2.x * v1.y);
}

// Get an angle from 2 vectors in 2D
float getAngleVector2(Vector2 v1, Vector2 v2)
{
    float cos = dotProductVector2(v1, v2) / (normVector2(v1) * normVector2(v2));
    float angle = acosf(cos);
    if (determinantVector2(v1, v2) >= 0)
    {
        return angle;
    }
    return -angle;
}

// Translate a point in 2D for a vector 'v'
Point2 translatePoint2(Point2 p, Vector2 v)
{
    Point2 new = {p.x + v.x, p.y + v.y};
    return new;
}

// Scale the point 'p' from 'h' for 'a'
Point2 homothetyPoint2(Point2 p, Point2 h, float a)
{
    Vector2 scale = multVector2(setVector2Point2(h, p), a);
    return translatePoint2(h, scale);
}

// Get 3 points from 2 vectors [1-3]
Point2 *getTriangleVector2(Point2 p, Vector2 v1, Vector2 v2)
{
    Point2 *summit = (Point2 *)malloc(sizeof(Point2) * 4);

    *(summit + 1) = p;
    *(summit + 2) = translatePoint2(p, v1);
    *(summit + 3) = translatePoint2(p, v2);

    return summit;
}

// Get 4 points from 2 vectors [1-4]
Point2 *getParallelogramVector2(Point2 p, Vector2 v1, Vector2 v2)
{
    Point2 *summit = (Point2 *)malloc(sizeof(Point2) * 5);

    *(summit + 1) = p;
    *(summit + 2) = translatePoint2(p, v1);
    *(summit + 3) = translatePoint2(p, v2);
    *(summit + 4) = translatePoint2(p, addVector2(v1, v2));

    return summit;
}

// 3D

int geogebra3dVScriptOpen(void)
{
    GEOGEBRA3V = fopen("toGeogebra3dV.txt", "w+");
    fprintf(GEOGEBRA3V, "L={");

    return 0;
}

int geogebra3dVScriptClose(char startVector, char endVector)
{
    if (!startVector)
        fclose(GEOGEBRA3V);
    fprintf(GEOGEBRA3V, "}\n\n");
    for (char c = startVector; c <= endVector; c++)
    {
        int i = c - startVector + 1;
        fprintf(GEOGEBRA3V, "%c=L(%d)\n", c, i);
    }
    fclose(GEOGEBRA3V);
    return 0;
}

// Print in Geogebra the coodinates of a Vector3 and if debug its name too in the console
void printVector3(Vector3 v, char str[])
{
    fprintf(GEOGEBRA3V, "%s(% .2f, % .2f, %.2f)", (*str == 'A') ? "" : ",", v.x, v.y, v.z);
    if (DEBUG_VECTORS >= 0)
        printf("Vector3 %s = (%.2f,%.2f,%.2f)\n", str, v.x, v.y, v.z);
}

// Set Vector in 3D
Vector3 setVector3(float x, float y, float z)
{
    Vector3 temp = {x, y, z};
    if (DEBUG_VECTORS == 1)
    {
        printf("New/Set/Reset Vector3 = (%.2f , %.2f, %.2f)\n", temp.x, temp.y, temp.z);
    }
    return temp;
}

// Set the opposite of vector3d v
Vector3 oppVector3(Vector3 v)
{
    Vector3 a = setVector3(-v.x, -v.y, -v.z);
    return a;
}

// Add the 2 vectors in 3D
Vector3 addVector3(Vector3 v1, Vector3 v2)
{
    return (Float3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

// Multiply a vector by a float
Vector3 multVector3(Vector3 v, float mul)
{
    return (Float3){v.x * mul, v.y * mul, v.z * mul};
}

// Dot product of 2 vectors in 3D
float dotProductVector3(Vector3 v1, Vector3 v2)
{
    return (float)(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// Cross product 2 vectors IN 3D, result is a vector
Vector3 crossProduct(Vector3 a, Vector3 b)
{
    Vector3 c = {a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x};
    return c;
}

// Get the circumcenter of the triangle in 3D
Point3 circumcenterTriangle3d(Point3 p1, Point3 p2, Point3 p3)
{
    Point3 U;
    Vector3 v1v2 = addVector3(p1, oppVector3(p2));
    Vector3 v1v3 = addVector3(p1, oppVector3(p3));
    Vector3 v2v3 = addVector3(p2, oppVector3(p3));
    Vector3 v2v1 = addVector3(p2, oppVector3(p1));
    Vector3 v3v1 = addVector3(p3, oppVector3(p1));
    Vector3 v3v2 = addVector3(p3, oppVector3(p2));
    float d = 2.0 * dotProductVector3(crossProduct(v1v2, v2v3), crossProduct(v1v2, v2v3));
    float a = dotProductVector3(v2v3, v2v3) * dotProductVector3(v1v2, v1v3) / d;
    float b = dotProductVector3(v1v3, v1v3) * dotProductVector3(v2v1, v2v3) / d;
    float c = dotProductVector3(v1v2, v1v2) * dotProductVector3(v3v1, v3v2) / d;
    U = addVector3(addVector3(multVector3(p1, a), multVector3(p2, b)), multVector3(p3, c));
    return U;
}