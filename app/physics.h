#pragma once

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#include <canvas.h>
#include <stdbool.h>

#include "Tools/axis.h"

static bool DEBUG_PHYSIC;

// SQT
typedef struct Sat
{
    Vector2 normal;
    float min;
    float max;
} Sat;

bool SAT_collision_SAT(Point2 poly1[], int sidesP1, Point2 poly2[], int sidesP2, bool displaySAT);
bool sphere_collision_SAT(Point2 center, float radius, Point2 poly[], int nbSegments, bool displaySAT);
Sat generate_SAT(Point2 p[], int nbSummit, int nbSegments);

// Collisions without SAT
bool sphere_collision_sphere(Point2 p1, float radius1, Point2 p2, float radius2);
bool point_collision_rectangle(Point2 p, float xmin, float ymin, float xmax, float ymax);
bool sphere_collision_rectangle(Point2 p, float radius, float xmin, float ymin, float xmax, float ymax);
void sphere_collision_border_replace(Point2 *p, float size, Point2 maxScreen);
void poly_collision_border_replace(Point2 p[], Point2 *center, int nbSides, float size, Point2 maxScreen);