#include "player.h"

#define DRAW_LIST igGetBackgroundDrawList_Nil()

void draw_thruster(Point2 p1, Point2 p2, Point2 p3)
{
    PathLineTo_point2(p1);
    PathLineTo_point2(p2);
    PathLineTo_point2(p3);
    ImDrawList_PathFillConvex(DRAW_LIST, igGetColorU32_Vec4((ImVec4){0.3f, 0.3f, 1.f, 0.5f}));
}
void anim_left_thruster(Player p)
{
    Point2 topThruster = midPoint2(p.shape[0], p.shape[1]);
    Point2 bottomThruster = midPoint2(topThruster, p.shape[1]);
    Point2 extThruster = rotatePoint2(bottomThruster, p.shape[0], -2 * M_PI / 3.f);
    draw_thruster(topThruster, bottomThruster, extThruster);
}

void anim_right_thruster(Player p)
{
    Point2 topThruster = midPoint2(p.shape[0], p.shape[9]);
    Point2 bottomThruster = midPoint2(topThruster, p.shape[9]);
    Point2 extThruster = rotatePoint2(bottomThruster, p.shape[0], 2 * M_PI / 3.f);
    draw_thruster(topThruster, bottomThruster, extThruster);
}

void anim_booster(Player p)
{
    Point2 leftBooster = p.shape[4];
    Point2 rightBooster = p.shape[6];
    Point2 extBooster = homothetyPoint2(p.shape[0], p.axis.origin, -1); //(p.shape[5], multVector2(p.axis.x, -p.size));
    draw_thruster(leftBooster, rightBooster, extBooster);
}

void anim_explode(Point2 origin, float maxsize, float deltaTime)
{
    float ct = 0;
    while (ct <= 1)
    {
        draw_bullet(origin, maxsize * deltaTime, CV_COL32(255, (int)(255 * ct), 0, 255));
        ct += deltaTime;
    }
}
// TODO : lerp for color change with deltaTime (alpha for blinking)
// Explosions
// Dismemberment ship