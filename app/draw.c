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

void draw_mine_body(Enemy *e)
{
    Point2 center = e->location.origin;
    float radius_big = get_max_size(e->size, e->type);
    float radius_small = get_small_size(e->size, e->type);

    float angle = e->angle;
    float rotation = M_PI / ((float)e->nbPoints / 2.f);

    for (int i = 0; i < e->nbPoints; i += 2)
    {
        e->points[i] = rotatePoint2(center, (Point2){center.x, center.y + radius_big}, angle);
        PathLineTo_point2(e->points[i]);
        angle += rotation;
        (e->points[i + 1]) = rotatePoint2(center, (Point2){center.x, center.y + radius_small}, angle);
        PathLineTo_point2((e->points[i + 1]));
        angle += rotation;
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_floating_mine(Enemy *e)
{
    draw_mine_body(e);
}

void draw_fireball_mine(Enemy *e)
{
    draw_mine_body(e);
    draw_fireball(e->location.origin, get_size_multiplier(e->size));
}

void draw_magnetic_mine(Enemy *e)
{
    draw_mine_body(e);
    draw_magnet(e->location.origin, 12, get_size_multiplier(e->size), e->angle + M_PI / 4.f, get_size_multiplier(e->size));
}

void draw_magnet_fire_mine(Enemy *e)
{
    Point2 center = e->location.origin;
    float sizeTop = get_max_size(e->size, e->type);
    float size = get_small_size(e->size, e->type);

    for (int i = 0; i < 8; i += 2)
    {
        float angle = M_PI * i / 4.f + e->angle;
        e->points[i] = rotatePoint2(center, (Point2){center.x + sizeTop, center.y}, angle);
        PathLineTo_point2(e->points[i]);
        e->points[i + 1] = rotatePoint2(center, (Point2){center.x + size, center.y}, angle);
        PathLineTo_point2(e->points[i + 1]);
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);

    draw_fireball(center, get_size_multiplier(e->size));
    draw_magnet(center, 14, 2.f * get_size_multiplier(e->size), e->angle + M_PI / 5.f, get_size_multiplier(e->size));
}

void draw_minelayer(Enemy *e)
{
    float x = e->location.origin.x;
    float y = e->location.origin.y;

    Point2 point[9] = {
        {x - 26.f, y + 10.f},
        {x - 36.f, y + 14.f},
        {x - 30.f, y},
        {x - 12.f, y},
        {x /*  */, y - 12.f},
        {x + 12.f, y},
        {x + 30.f, y},
        {x + 36.f, y + 14.f},
        {x + 26.f, y + 10.f}};

    for (int i = 0; i < 9; i++)
    {
        e->points[i] = rotatePoint2(e->location.origin, point[i], e->angle);
        PathLineTo_point2(e->points[i]);
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

// draw enemy depending on its type & size (see enemyType & enemySize)
int draw_any_enemy(Enemy *e)
{
    drawList = igGetBackgroundDrawList_Nil();
    color = igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 1.0f}));
    switch (e->type)
    {
    case FLOATING:
        draw_floating_mine(e);
        return 0;

    case FIREBALL_MINE:
        draw_fireball_mine(e);
        return 0;

    case MAGNETIC:
        draw_magnetic_mine(e);
        return 0;

    case MAGNET_FIRE:
        draw_magnet_fire_mine(e);
        return 0;

    case FIREBALL:
        draw_fireball(e->location.origin, get_max_size(e->size, e->type));
        return 0;

    case MINELAYER:
        draw_minelayer(e);
        return 0;
    }
    return -1;
}

void draw_player_draw(Player p)
{
    float x = p.axis.origin.x;
    float y = p.axis.origin.y;

    Point2 point[10] = {
        {x /*  */, y - 40.f},
        {x + 10.f, y},
        {x + 15.f, y},
        {x + 20.f, y + 30.f},
        {x + 07.5, y + 10.f},
        {x + 00.f, y + 20.f},
        {x - 07.5, y + 10.f},
        {x - 20.f, y + 30.f},
        {x - 15.f, y},
        {x - 10.f, y}};

    for (int i = 0; i < 10; i++)
        PathLineTo_point2(point[i]);
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}
