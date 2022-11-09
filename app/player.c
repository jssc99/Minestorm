#include "player.h"
#include <canvas.h>

const bool DEBUG_PLAYER = true;
const float MAX_SPEED_SHIP = 0.5;
const float ACCELERATION = 0.01;

// Initialize Player at position(x,y) First time think about setting lives to 3.
Player player_init(Player p, float x, float y, float size)
{
    p = (Player){0};
    p.axis = (Axis2){{x, y},
                     {0, -size},
                     {size, 0}};
    p.moveLine = p.axis.x;
    p.targetLine = p.moveLine;
    p.size = size;
    return p;
}

void draw_circle(Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color)
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
    draw_circle(origin, 3, sz, getAngleVector2(p.axis.x, (Float2){0, 1}), color);
    if (DEBUG_PLAYER)
    {
        Point2 x = addVector2(origin, multVector2(p.axis.x, 2.0));
        Point2 y = addVector2(origin, multVector2(p.axis.y, 2.0));
        Point2 z = addVector2(origin, multVector2(p.moveLine, 2.0));
        Point2 dz = addVector2(origin, multVector2(p.moveLine, 2.0+(p.speed*3.0)));
        cvAddLine(origin.x, origin.y, dz.x, dz.y, CV_COL32(0, 0, 255, 255));  // speed
        cvAddLine(origin.x, origin.y, z.x, z.y, CV_COL32(255, 0, 255, 255)); //  moveline
        cvAddLine(origin.x, origin.y, x.x, x.y, CV_COL32(255, 0, 0, 255));  // X axis aka targetline
        cvAddLine(origin.x, origin.y, y.x, y.y, CV_COL32(0, 255, 0, 255)); //  Y axis
    }
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

Player update_player(Player p)
{
    p.targetLine = p.axis.x; // multVector2(addVector2(p.axis.x,p.axis.y), 0.5);
    //Deceleration
    p.inertia = multVector2(p.inertia,0.99); 
    p.axis = translateAxis2(p.axis, p.inertia);
    //p.axis = translateAxis2(p.axis, multVector2(p.moveLine, p.speed));
    //Deceleration
    p.speed *= 0.99;
    return p;
}
// Turn the player to the left igIsKeyDown(ImGuiKey_D)
Player turnleft_player(Player p)
{
    p = rotate_player(p, -M_PI / 24);
    // p.moveLine = p.axis.x;
    return p;
}

// Turn the player to the right igIsKeyDown(ImGuiKey_G)
Player turnright_player(Player p)
{
    p = rotate_player(p, M_PI / 24);
    // p.moveLine = p.axis.x;
    return p;
}

Player accelerate_player(Player p)
{
    if (p.speed < MAX_SPEED_SHIP)
        p.speed += ACCELERATION;
    p.moveLine = p.targetLine;
    p.inertia = addVector2(p.inertia,multVector2(p.moveLine,ACCELERATION));
    return p;
}