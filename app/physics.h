
#include <stdbool.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <canvas.h>
#include "Tools/axis.h"

//Collisions
bool SS_collision_SS(Point2 p1, float size1, Point2 p2, float size2);
bool point_collision_rectangle(Point2 p, float xmin, float ymin, float xmax, float ymax);
bool SS_collision_rectangle(Point2 p, float size, float xmin, float ymin, float xmax, float ymax);