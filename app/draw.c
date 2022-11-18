#include "enemy.h"
#include "player.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#define GREY igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 0.5}))
#define WHITE igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 1.0f}))
#define PLAYER1 igGetColorU32_Vec4(((ImVec4){0.0f, 0.8f, 1.0f, 1.0f}))
#define PLAYER2 igGetColorU32_Vec4(((ImVec4){0.0f, 1.0f, 0.5f, 1.0f}))

#define DRAW_LIST igGetBackgroundDrawList_Nil()

void PathLineTo_point2(Point2 p)
{
    ImDrawList_PathLineTo(DRAW_LIST, (ImVec2){p.x, p.y});
}

void draw_poly(Point2 *points, int nbPoints)
{
    for (int i = 0; i < nbPoints; i++)
        PathLineTo_point2(points[i]);
    ImDrawList_PathStroke(DRAW_LIST, WHITE, ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_fireball(Point2 center, float radius)
{
    ImDrawList_AddCircle(DRAW_LIST, (ImVec2){center.x, center.y}, radius, WHITE, NB_POINTS_FIREBALL, 1.5f);
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
    ImDrawList_PathStroke(DRAW_LIST, WHITE, ImDrawFlags_RoundCornersAll, 1.5f);
}

void draw_baby_mine(Enemy *e)
{
    float radius = 4.f;
    float x = e->location.origin.x;
    float y = e->location.origin.y;
    ImDrawList_AddLine(DRAW_LIST, (ImVec2){x - radius, y - radius}, (ImVec2){x + 0.5f + radius, y + radius}, GREY, 1.5f);
    ImDrawList_AddLine(DRAW_LIST, (ImVec2){x - radius, y + radius}, (ImVec2){x + 0.5f + radius, y - radius}, GREY, 1.5f);
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

ImU32 get_player_color(int playerNb)
{
    if (playerNb == 1)
        return PLAYER1;
    else
        return PLAYER2;
}
//Draw the player
void draw_player(Player p, int playerNb)
{
    float x = p.axis.origin.x;
    float y = p.axis.origin.y;

    for (int i = 0; i < 10; i++)
        PathLineTo_point2(p.shape[i]);
    ImDrawList_PathStroke(DRAW_LIST, get_player_color(playerNb), ImDrawFlags_Closed || ImDrawFlags_RoundCornersAll, 1.5f);
}

// draw 1 bullet
void draw_bullet(Point2 center, float radius, unsigned int color)
{
    ImDrawList_AddCircleFilled(DRAW_LIST, (ImVec2){center.x,center.y}, radius, color, 50);
}

// Draw debug
void draw_debug_player(Player *p)
{
    Point2 origin = p->axis.origin;
    Point2 x = addVector2(origin, multVector2(p->axis.x, p->size * 2.0));
    Point2 y = addVector2(origin, multVector2(p->axis.y, p->size * 2.0));
    Point2 z = addVector2(origin, multVector2(p->inertia, p->size *2/ MAX_SPEED_SHIP));
    Point2 dz = addVector2(p->shape[0], multVector2(p->moveLine, p->speed *2 / MAX_SPEED_SHIP));

    if (p->displayInertia)
        cvAddLine(origin.x, origin.y, z.x, z.y, CV_COL32(255, 0, 255, 255)); //  inertia
    if (p->displaySpeed)
        cvAddLine(p->shape[0].x, p->shape[0].y, dz.x, dz.y, CV_COL32(0, 0, 255, 255)); // speed * moveline
    if (p->displayAxis)
    {
        cvAddLine(origin.x, origin.y, x.x, x.y, CV_COL32(255, 0, 0, 255)); // X axis aka targetline *2
        cvAddLine(origin.x, origin.y, y.x, y.y, CV_COL32(0, 255, 0, 255)); //  Y axis
    }
    if (p->displaySSphere)
    {
        ImDrawList_AddCircle(DRAW_LIST, (ImVec2){origin.x, origin.y}, p->size, CV_COL32(255, 255, 255, 200), 50, 0.5f); // Surrounding sphere
        ImDrawList_AddCircle(DRAW_LIST, (ImVec2){500, 400}, 15, CV_COL32(255, 255, 255, 200), 50, 0.5f);
    }
}