#include "player.h"

#define DRAW_LIST igGetBackgroundDrawList_Nil()

void anim_left_thruster(Player *p)
{
    Point2 topThruster = midPoint2(p->shape[0], p->shape[1]);
    Point2 bottomThruster = midPoint2(topThruster, p->shape[1]);
    Point2 extThruster = rotatePoint2(bottomThruster, topThruster,M_PI/3);
    PathLineTo_point2(topThruster);
    PathLineTo_point2(bottomThruster);
    PathLineTo_point2(extThruster);
    ImDrawList_PathStroke(DRAW_LIST, igGetColorU32_Vec4(((ImVec4){1.f, 1.f, 1.0f, 1.f})), ImDrawFlags_Closed, 1.5f);
}

//TODO : lerp for color change with deltaTime (alpha for blinking)
//Explosions
//Dismemberment ship