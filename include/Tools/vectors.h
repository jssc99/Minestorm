#pragma once

#include "points.h"

typedef Float2 Vector2;
typedef Float3 Vector3;

int geogebraVScriptOpen(void);
int geogebraVScriptClose(char startVector, char endVector);
Vector2 setVector2(float x, float y);
Vector2 setVector2Point2(Point2 p1, Point2 p2);
Vector2 oppVector2(Vector2 v);
float normVector2(Vector2 v);
Vector2 addVector2(Vector2 v1, Vector2 v2);
Vector2 multVector2(Vector2 v, float mul);
Vector2 normalizedVector2(Vector2 v);
Vector2 getNormalVectorVector2(Vector2 v);
Vector2 getNormalVectorPoint2(Point2 a, Point2 b);
float dotProductVector2(Vector2 v1, Vector2 v2);
float determinantVector2(Vector2 v1, Vector2 v2);
float getAngleVector2(Vector2 v1, Vector2 v2);
Point2 translatePoint2(Point2 p, Vector2 v);
Point2 homothetyPoint2(Point2 p, Point2 h, float a);
Point2 *getTriangleVector2(Point2 p, Vector2 v1, Vector2 v2);
Point2 *getParallelogramVector2(Point2 p, Vector2 v1, Vector2 v2);
void printVector2(Vector2, char str[]);

// 3D
int geogebra3dVScriptOpen(void);
int geogebra3dVScriptClose(char startVector, char endVector);
void printVector3(Vector3 v, char str[]);
Vector3 setVector3(float x, float y, float z);
Vector3 oppVector3(Vector3 v);
Vector3 addVector3(Vector3 v1, Vector3 v2);
Vector3 multVector3(Vector3 v, float mul);
float dotProductVector3(Vector3 v1, Vector3 v2);
Vector3 crossProduct(Vector3 a, Vector3 b);
Point3 circumcenterTriangle3d(Point3 p1, Point3 p2, Point3 p3);