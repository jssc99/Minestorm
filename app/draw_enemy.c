#include "enemy.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

static ImDrawList *drawList;
static ImU32 color;

void PathLineTo_point2(Point2 p)
{
    ImDrawList_PathLineTo(drawList, (ImVec2){p.x, p.y});
}

void draw_fireball(Point2 center, int sides, float radius)
{
    ImDrawList_AddCircle(drawList, (ImVec2){center.x, center.y}, radius, color, sides, 1.5f);
}

void draw_magnet(Point2 center, int sides, float radius, float angleOffset, float size)
{
    sides++;
    for (int i = 0; i < sides; i++)
    {
        float angle = -M_PI * (float)i / (float)(sides - 1) + angleOffset;
        Point2 point = rotatePoint2(center, (Point2){-radius + center.x, center.y}, angle);
        if (i == 0)
            PathLineTo_point2(rotatePoint2(point, (Point2){point.x, point.y - size * 1.6}, angle));
        PathLineTo_point2(point);
        if (i == (sides - 1))
            PathLineTo_point2(rotatePoint2(point, (Point2){point.x, point.y + size * 1.6}, angle));
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_mine_body(Point2 center, float radius_big, float radius_small, float angleOffset, int tops)
{
    float angle = angleOffset;
    float rotation = M_PI / (float)tops;
    for (int i = 0; i < tops; i++)
    {
        PathLineTo_point2(rotatePoint2(center, (Point2){center.x, center.y + radius_big}, angle));
        angle += rotation;
        PathLineTo_point2(rotatePoint2(center, (Point2){center.x, center.y + radius_small}, angle));
        angle += rotation;
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_floating_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   get_max_size(enemy.size, enemy.type),
                   FLOATING_SMALL_RADIUS * get_size_multiplier(enemy.size),
                   enemy.angle, 3);
}

void draw_fireball_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   get_max_size(enemy.size, enemy.type),
                   FIREBALL_MINE_SMALL_RADIUS * get_size_multiplier(enemy.size),
                   enemy.angle, 4);
    draw_fireball((Point2)enemy.location.origin, 20, get_size_multiplier(enemy.size));
}

void draw_magnetic_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   get_max_size(enemy.size, enemy.type),
                   MAGNETIC_SMALL_RADIUS * get_size_multiplier(enemy.size),
                   enemy.angle, 4);
    draw_magnet((Point2)enemy.location.origin, 12, get_size_multiplier(enemy.size), enemy.angle + M_PI / 4.f, get_size_multiplier(enemy.size));
}

void draw_magnet_fire_mine(Enemy enemy)
{
    float sizeTop = get_max_size(enemy.size, enemy.type);
    float size = MAGNET_FIRE_SMALL_RADIUS * get_size_multiplier(enemy.size);
    Point2 center = {enemy.location.origin.x, enemy.location.origin.y};

    for (int i = 0; i < 4; i++)
    {
        float angle = M_PI * (float)i / 2.f + enemy.angle;
        PathLineTo_point2(rotatePoint2(center, (Point2){size + center.x, center.y}, angle));
        PathLineTo_point2(rotatePoint2(center, (Point2){sizeTop + center.x, center.y}, angle));
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);

    draw_fireball((Point2)enemy.location.origin, 20, get_size_multiplier(enemy.size));
    draw_magnet((Point2)enemy.location.origin, 12, 2.f * get_size_multiplier(enemy.size), enemy.angle + M_PI / 4.f, get_size_multiplier(enemy.size));
}

void draw_minelayer(Enemy enemy)
{
    Point2 center = enemy.location.origin;
    float size = get_size_multiplier(enemy.size);

    Point2 point[9] = {
        {center.x - 09.f * size, center.y + 2.f * size},
        {center.x - 11.f * size, center.y + 4.f * size},
        {center.x - 10.f * size, center.y},
        {center.x - 02.f * size, center.y},
        {center.x, center.y - 2.f * size},
        {center.x + 02.f * size, center.y},
        {center.x + 10.f * size, center.y},
        {center.x + 11.f * size, center.y + 4.f * size},
        {center.x + 09.f * size, center.y + 2.f * size}};

    for (int i = 0; i < 9; i++)
        PathLineTo_point2(rotatePoint2(center, point[i], enemy.angle));
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

// draw enemy depending on its type & size (see enemyType & enemySize)
int draw_any_enemy(Enemy enemy)
{
    drawList = igGetBackgroundDrawList_Nil();
    color = igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 1.0f}));
    switch (enemy.type)
    {
    case FLOATING:
        draw_floating_mine(enemy);
        return 0;

    case FIREBALL_MINE:
        draw_fireball_mine(enemy);
        return 0;

    case MAGNETIC:
        draw_magnetic_mine(enemy);
        return 0;

    case MAGNET_FIRE:
        draw_magnet_fire_mine(enemy);
        return 0;

    case FIREBALL:
        draw_fireball((Point2)enemy.location.origin, 10, get_max_size(enemy.size, enemy.type));
        return 0;

    case MINELAYER:
        draw_minelayer(enemy);
        return 0;
    }
    return -1;
}
