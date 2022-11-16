#include "enemy.h"
#include "player.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

static ImDrawList *drawList;
static ImU32 color;

void PathLineTo_point2(Point2 p)
{
    ImDrawList_PathLineTo(drawList, (ImVec2){p.x, p.y});
}

void draw_mine(Point2 *points, int nbPoints)
{
    for (int i = 0; i < nbPoints; i++)
        PathLineTo_point2(points[i]);
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_fireball(Point2 center, float radius)
{
    ImDrawList_AddCircle(drawList, (ImVec2){center.x, center.y}, radius, color, NB_POINTS_FIREBALL, 1.5f);
}

void draw_magnet(Point2 center, int sides, float radius, float angleOffset, float length)
{
    sides++;
    for (int i = 0; i < sides; i++)
    {
        float angle = -M_PI * (float)i / (float)(sides - 1) + angleOffset;
        Point2 point = rotatePoint2(center, (Point2){-radius + center.x, center.y}, angle);
        if (i == 0)
            PathLineTo_point2(rotatePoint2(point, (Point2){point.x, point.y - length * 1.6}, angle));
        PathLineTo_point2(point);
        if (i == (sides - 1))
            PathLineTo_point2(rotatePoint2(point, (Point2){point.x, point.y + length * 1.6}, angle));
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_RoundCornersAll, 1.5f);
}

// draw enemy depending on its type & size (see enemyType & enemySize)
void draw_any_enemy(Enemy *e)
{
    drawList = igGetBackgroundDrawList_Nil();
    color = igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 1.0f}));

    if (e->type != FIREBALL)
        draw_mine(e->points, e->nbPoints);
    if (e->type == FIREBALL_MINE || e->type == MAGNET_FIRE)
        draw_fireball(e->location.origin, get_size_multiplier(e->size));
    if (e->type == FIREBALL)
        draw_fireball(e->location.origin, 0.5 * get_size_multiplier(e->size));
    if (e->type == MAGNETIC)
        draw_magnet(e->location.origin, 16, get_size_multiplier(e->size), e->angle + M_PI / 4.f, get_size_multiplier(e->size));
    if (e->type == MAGNET_FIRE)
        draw_magnet(e->location.origin, 16, 2.f * get_size_multiplier(e->size), e->angle + M_PI / 5.f, get_size_multiplier(e->size));
}

void draw_player_draw(Player p)
{
    float x = p.axis.origin.x;
    float y = p.axis.origin.y;

    Point2 point[10] = {
        {x /*  */, y - 30.f},
        {x + 05.f, y},
        {x + 10.f, y},
        {x + 15.f, y + 20.f},
        {x + 04.2, y + 07.f},
        {x /*  */, y + 12.f},
        {x - 04.2, y + 07.f},
        {x - 15.f, y + 20.f},
        {x - 10.f, y},
        {x - 05.f, y}};

    for (int i = 0; i < 10; i++)
        PathLineTo_point2(point[i]);
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}
