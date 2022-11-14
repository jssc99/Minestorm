#include <stdbool.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <canvas.h>

#include "Tools/axis.h"

// Sat
typedef struct Sat
{
    Vector2 normal;
    float min;
    float max;
} Sat;

bool sphere_collision_SAT(Point2 center, float radius, Sat* range, int nbSegments);
void generate_SAT(Sat* range, Point2 *p, int nbSegments);

// Collisions
bool sphere_collision_sphere(Point2 p1, float radius1, Point2 p2, float radius2);
bool point_collision_rectangle(Point2 p, float xmin, float ymin, float xmax, float ymax);
bool sphere_collision_rectangle(Point2 p, float radius, float xmin, float ymin, float xmax, float ymax);