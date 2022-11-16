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

void draw_mine_body(Enemy enemy)
{
    Point2 center = enemy.location.origin;
    float radius_big = get_max_size(enemy.size, enemy.type);
    float radius_small = get_small_size(enemy.size, enemy.type);

    float angle = enemy.angle;
    float rotation = M_PI / ((float)enemy.nbPoints / 2.f);
    
    for (int i = 0; i < enemy.nbPoints; i += 2)
    {
        enemy.points[i] = rotatePoint2(center, (Point2){center.x, center.y + radius_big}, angle);
        PathLineTo_point2(enemy.points[i]);
        angle += rotation;
        enemy.points[i + 1] = rotatePoint2(center, (Point2){center.x, center.y + radius_small}, angle);
        PathLineTo_point2(enemy.points[i + 1]);
        angle += rotation;
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_floating_mine(Enemy enemy)
{
    draw_mine_body(enemy);
}

void draw_fireball_mine(Enemy enemy)
{
    draw_mine_body(enemy);
    draw_fireball(enemy.location.origin, 20, get_size_multiplier(enemy.size));
}

void draw_magnetic_mine(Enemy enemy)
{
    draw_mine_body(enemy);
    draw_magnet(enemy.location.origin, 12, get_size_multiplier(enemy.size), enemy.angle + M_PI / 4.f, get_size_multiplier(enemy.size));
}

void draw_magnet_fire_mine(Enemy enemy)
{
    float sizeTop = get_max_size(enemy.size, enemy.type);
    float size = get_small_size(enemy.size, enemy.type);
    float x = enemy.location.origin.x;
    float y = enemy.location.origin.y;

    Point2 point[8] = {
        {x + sizeTop, y},
        {x + size, y},
        {x /*      */, y + sizeTop},
        {x /*      */, y + size},
        {x - sizeTop, y},
        {x - size, y},
        {x /*      */, y - sizeTop},
        {x /*      */, y - size}};

    for (int i = 0; i < 8; i++)
    {
        enemy.points[i] = rotatePoint2(enemy.location.origin, point[i], enemy.angle);
        PathLineTo_point2(enemy.points[i]);
    }
    ImDrawList_PathStroke(drawList, color, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);

    draw_fireball(enemy.location.origin, 20, get_size_multiplier(enemy.size));
    draw_magnet(enemy.location.origin, 14, 2.f * get_size_multiplier(enemy.size), enemy.angle + M_PI / 5.f, get_size_multiplier(enemy.size));
}

void draw_minelayer(Enemy enemy)
{
    float x = enemy.location.origin.x;
    float y = enemy.location.origin.y;

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
        enemy.points[i] = rotatePoint2(enemy.location.origin, point[i], enemy.angle);
        PathLineTo_point2(enemy.points[i]);
    }
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
        draw_fireball((Point2)enemy.location.origin, 12, 4.f);
        return 0;

    case MINELAYER:
        draw_minelayer(enemy);
        return 0;
    }
    return -1;
}

void draw_player_draw(Player player)
{
    float x = player.axis.origin.x;
    float y = player.axis.origin.y;

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
