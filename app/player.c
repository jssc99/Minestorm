#include "player.h"
#include <canvas.h>

// Initialize Player at position(x,y) First time think about setting lives to 3.
Player player_init(Player p, float x, float y)
{
    p.axis = (Axis2){{x, y},
                     {40, 0},
                     {0, 40}};
    p.moveLine = p.axis.y;
    p.targetLine = p.moveLine;
    p.speed = 0;
    return p;
}

void draw_Polygon(Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color)
{
    Point2 point[sides];
    point[0].x = radius * sinf(angleOffset) + center.x;
    point[0].y = radius * cosf(angleOffset) + center.y;
    cvPathLineTo(point[0].x, point[0].y);

    float angle = M_PI * 2 / (float)sides;
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

// Draw the player
void draw_player(Player p, unsigned int color, float sz)
{
    Point2 origin = p.axis.origin;
    Point2 x = addVector2(origin, p.axis.x);
    Point2 y = addVector2(origin, p.axis.y);
    Point2 z = addVector2(origin, p.targetLine);
    // sz = sz * normVector2(addVector2(p.axis.x, p.axis.y)); // Test
    draw_Polygon(origin, 3, sz, -M_PI/6 + getAngleVector2(p.axis.y,(Float2){1,0}), color);
    cvAddLine(origin.x, origin.y, x.x, x.y, CV_COL32(0, 255, 0, 255));
    cvAddLine(origin.x, origin.y, y.x, y.y, CV_COL32(0, 0, 255, 255));
    cvAddLine(origin.x, origin.y, z.x, z.y, CV_COL32(255, 0, 0, 255));
    // cvAddLine(p.axis.origin.x, p.axis.origin.y, p.axis.origin.x + 30 * p.axis.y.x, p.axis.origin.x + 30 * p.axis.y.y, CV_COL32(0, 0, 255, 255));
    //  cvAddLine(400, 300, 440, 300, CV_COL32(255, 0, 0, 255));
    //  cvAddLine(400, 300, 400, 330, CV_COL32(0, 0, 255, 255));
    //      AddTriangle(ImVec2(p.axis.x+sz*0.5f, p.axis.y), ImVec2(p.axis.x+sz,p.axis.y+sz-0.5f), ImVec2(p.axis.x,p.axis.y+sz-0.5f), color, th); x += sz+spacing;
    return;
}

// Rotate the player
Player rotate_player(Player p, float angle)
{
    p.axis = rotateAxis2(p.axis, angle);
    return p;
}

Player player_update(Player p)
{
    p.targetLine = p.axis.y; // multVector2(addVector2(p.axis.x,p.axis.y), 0.5);
    //p.moveLine = p.targetLine;
    p.axis = translateAxis2(p.axis, multVector2(p.moveLine, p.speed));
    p.speed *= 0.99;
    return p;
}