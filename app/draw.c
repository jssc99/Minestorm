#pragma once

#include "enemy.h"
#include "player.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#define GREY igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 0.5}))
#define WHITE igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 1.0f}))
#define PLAYER1 igGetColorU32_Vec4(((ImVec4){0.0f, 0.8f, 1.0f, 1.0f}))
#define PLAYER2 igGetColorU32_Vec4(((ImVec4){0.0f, 1.0f, 0.5f, 1.0f}))

#define drawList igGetBackgroundDrawList_Nil()

// static ImDrawList *drawList;

void PathLineTo_point2(Point2 p)
{
    ImDrawList_PathLineTo(drawList, (ImVec2){p.x, p.y});
}

void draw_poly(Point2 *points, int nbPoints)
{
    for (int i = 0; i < nbPoints; i++)
        PathLineTo_point2(points[i]);
    ImDrawList_PathStroke(drawList, WHITE, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_fireball(Point2 center, float radius)
{
    ImDrawList_AddCircle(drawList, (ImVec2){center.x, center.y}, radius, WHITE, NB_POINTS_FIREBALL, 1.5f);
}

void draw_magnet(Point2 center, int sides, float radius, float angleOffset, float length)
{
    for (int i = 0; i < (sides + 1); i++)
    {
        float angle = -M_PI * (float)i / (float)sides + angleOffset;
        Point2 point = rotatePoint2(center, (Point2){-radius + center.x, center.y}, angle);
        if (i == 0)
            PathLineTo_point2(rotatePoint2(point, (Point2){point.x, point.y - length * 1.6}, angle));
        PathLineTo_point2(point);
        if (i == sides)
            PathLineTo_point2(rotatePoint2(point, (Point2){point.x, point.y + length * 1.6}, angle));
    }
    ImDrawList_PathStroke(drawList, WHITE, ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_baby_mine(Enemy *e)
{
    float radius = 4.f;
    float x = e->location.origin.x;
    float y = e->location.origin.y;
    ImDrawList_AddLine(drawList, (ImVec2){x - radius, y - radius}, (ImVec2){x + 0.5f + radius, y + radius}, GREY, 1.5f);
    ImDrawList_AddLine(drawList, (ImVec2){x - radius, y + radius}, (ImVec2){x + 0.5f + radius, y - radius}, GREY, 1.5f);
}

// draw enemy depending on its type & size (see enemyType & enemySize)
void draw_any_enemy(Enemy *e)
{
    if (e->status == CHILD)
        draw_baby_mine(e);
    else
    {
        if (e->type != FIREBALL)
            draw_poly(e->points, e->nbPoints);
        if (e->type == FIREBALL_MINE || e->type == MAGNET_FIRE)
            draw_fireball(e->location.origin, get_size_multiplier(e->size));
        if (e->type == FIREBALL)
            draw_fireball(e->location.origin, 0.5 * get_size_multiplier(e->size));
        if (e->type == MAGNETIC)
            draw_magnet(e->location.origin, 16, get_size_multiplier(e->size), e->angle - M_PI / 2.f, get_size_multiplier(e->size));
        if (e->type == MAGNET_FIRE)
            draw_magnet(e->location.origin, 16, 2.f * get_size_multiplier(e->size), e->angle - M_PI / 2.f, get_size_multiplier(e->size));
    }
}

ImU32 get_player_color(playerNb)
{
    if (playerNb == 1)
        return PLAYER1;
    else
        return PLAYER2;
}

void draw_player(Player p, int playerNb)
{
    float x = p.axis.origin.x;
    float y = p.axis.origin.y;

    for (int i = 0; i < 10; i++)
        PathLineTo_point2(p.shape[i]);
    ImDrawList_PathStroke(drawList,
                          get_player_color(playerNb),
                          ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll,
                          1.5f);
}
