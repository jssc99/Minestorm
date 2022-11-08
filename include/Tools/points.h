#pragma once
#include <math.h>
typedef struct Float2
{
    float x;
    float y;
} Float2;

typedef Float2 Point2;

typedef struct Triangle2
{
    Point2 p1;
    Point2 p2;
    Point2 p3;
    float side1;
    float side2;
    float side3;
} Triangle2;

Point2 setPoint2(float x, float y);
void printPoint2(Point2 p, char str[]);
void printGeogebraPoint2(Point2 p);
int geogebraScriptOpen(void);
int geogebraScriptClose(char startPoint, char endPoint);
Point2 oppPoint2(Point2 p);
Point2 midPoint2(Point2 p1, Point2 p2);
Point2 rotatePoint2(Point2 center, Point2 origin, float angle);
float distPoint2(Point2 p1, Point2 p2);
float *getTriangleSidePoints2(Point2 p1, Point2 p2, Point2 p3);
float getTriangleSurfacePoints2(Point2 p1, Point2 p2, Point2 p3);
int isTriangleSpecialPoints2(Point2 p1, Point2 p2, Point2 p3);
Point2 isobarycenterTriangle(Point2 p1, Point2 p2, Point2 p3);
Point2 circumcenterTriangle(Point2 p1, Point2 p2, Point2 p3);
float perimeterCircle(Point2 a, Point2 b);
float surfaceCircle(Point2 a, Point2 b);
float *getQuadrilateralSidePoints2(Point2 p1, Point2 p2, Point2 p3, Point2 p4);
float getQuadrilateralSurfacePoints2(Point2 p1, Point2 p2, Point2 p3, Point2 p4);

// 3D
typedef struct Float3
{
    float x;
    float y;
    float z;
} Float3;
typedef Float3 Point3;

Point3 setPoint3(float x, float y, float z);
void printPoint3(Point3 p, char str[]);
void printGeogebraPoint3(Point3 p);
int geogebra3dScriptOpen(void);
int geogebra3dScriptClose(char startPoint, char endPoint);
Point3 oppPoint3(Point3 p);
Point3 midPoint3(Point3 p1, Point3 p2);
float distPoint3(Point3 p1, Point3 p2);
Point3 rotateHorPoint3(Point3 center, Point3 origin, float phi);
Point3 rotateVerPoint3(Point3 center, Point3 origin, float theta);
Point3 rotatePoint3(Point3 center, Point3 origin, float theta, float phi);
float *getTriangleSidePoint3(Point3 p1, Point3 p2, Point3 p3);
float getTriangleSurfacePoint3(Point3 p1, Point3 p2, Point3 p3);
int isTriangleSpecialPoint3(Point3 p1, Point3 p2, Point3 p3);
Point3 isobarycenterTrianglePoint3(Point3 p1, Point3 p2, Point3 p3);
