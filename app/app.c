#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#include <canvas.h>

#include "app.h"

void appInit(App *app)
{
    *app = (App){0};
}

void drawPolygon(Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color)
{
    Point2 point[sides];
    point[0].x = radius * sinf(angleOffset) + center.x;
    point[0].y = radius * cosf(angleOffset) + center.y;
    cvPathLineTo(point[0].x, point[0].y);

    float angle = PI * 2 / (float)sides;
    float baseX = point[0].x;
    float baseY = point[0].y;
    for (int i = 1; i < sides; i++)
    {
        float c = cosf(angle * i);
        float s = sinf(angle * i);
        point[i].x = (baseX - center.x) * c - (baseY - center.y) * s + center.x;
        point[i].y = (baseX - center.x) * s + (baseY - center.y) * c + center.y;
        cvPathLineTo(point[i].x, point[i].y);
    }
    cvPathStroke(color, 1);
}

void appUpdate(App *app)
{
    ImGuiIO *io = igGetIO();
    int width = io->DisplaySize.x;
    int height = io->DisplaySize.y;
    cvSetCoordinateSystemFromScreenSpace(
        0, 0,      // origin
        10.f, 0.f, // right
        0.f, 10.f  // top
    );
}

void appShutdown(App *app)
{
    (void)app; // TOREMOVE: Silence unused parameter ‘app’ warning
}