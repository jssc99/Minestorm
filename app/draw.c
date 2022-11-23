#include "enemy.h"
#include "player.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#define GREY igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 0.5}))
#define WHITE igGetColorU32_Vec4(((ImVec4){1.0f, 1.0f, 1.0f, 1.0f}))
#define PLAYER1 igGetColorU32_Vec4(((ImVec4){0.0f, 0.8f, 1.0f, 1.0f}))
#define PLAYER2 igGetColorU32_Vec4(((ImVec4){0.0f, 1.0f, 0.5f, 1.0f}))

#define DRAW_LIST igGetBackgroundDrawList_Nil()

// Essier to use PathLineTo (by using a point)
void PathLineTo_point2(Point2 p)
{
    ImDrawList_PathLineTo(DRAW_LIST, (ImVec2){p.x, p.y});
}

// Draw any polygon
void draw_poly(Point2 *points, int nbPoints)
{
    for (int i = 0; i < nbPoints; i++)
        PathLineTo_point2(points[i]);
    ImDrawList_PathStroke(DRAW_LIST, WHITE, ImDrawFlags_Closed | ImDrawFlags_RoundCornersAll, 1.5f);
}

// Draw a fireball
void draw_fireball(Point2 center, float radius)
{
    ImDrawList_AddCircle(DRAW_LIST, (ImVec2){center.x, center.y}, radius, WHITE, NB_POINTS_FIREBALL, 1.5f);
}

// Draw a magnet
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

// Draw a baby mine
void draw_baby_mine(Enemy e)
{
    float radius = 4.f;
    float x = e.location.origin.x;
    float y = e.location.origin.y;
    ImDrawList_AddLine(DRAW_LIST, (ImVec2){x - radius, y - radius}, (ImVec2){x + 0.5f + radius, y + radius}, GREY, 1.5f);
    ImDrawList_AddLine(DRAW_LIST, (ImVec2){x - radius, y + radius}, (ImVec2){x + 0.5f + radius, y - radius}, GREY, 1.5f);
}

// draw an enemy depending on its type & size (see enemyType & enemySize)
void draw_any_enemy(Enemy *e)
{
    if (e->status == CHILD)
        draw_baby_mine(*e);
    else if (e->status == ADULT)
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

// draw all enemy (calls draw_any_enemy())
void draw_all_enemy(Enemy e[], int size)
{
    for (int i = 0; i < size; i++)
        draw_any_enemy(&(e[i]));
}

// Mini-func to get player color
ImU32 get_player_color(int playerId)
{
    if (playerId == 1)
        return PLAYER1;
    else
        return PLAYER2;
}

// Draw a player
void draw_player(Player p, int playerId)
{
    for (int i = 0; i < 10; i++)
        PathLineTo_point2(p.shape[i]);
    ImDrawList_PathStroke(DRAW_LIST, get_player_color(playerId), ImDrawFlags_Closed | ImDrawFlags_RoundCornersAll, 2.f);
}

// Draw a bullet
// // Can also serve as a more flexible fireball (due to color)
void draw_bullet(Point2 center, float radius, unsigned int color)
{
    ImDrawList_AddCircleFilled(DRAW_LIST, (ImVec2){center.x, center.y}, radius, color, 50);
}
// Draw ALL the bullets of a player
void draw_players_bullets(Player *p)
{
    for (int i = 0; i < MAX_BULLETS; i++)
        if (p->bullets[i].lifespan)
            draw_bullet(p->bullets[i].location, p->bullets[i].size, CV_COL32(255, 255 / MAX_BULLETS * i, 0, 255));
}
// Draw debug menu (player)
void draw_debug_player(Player *p)
{
    Point2 origin = p->axis.origin;
    Point2 x = addVector2(origin, multVector2(p->axis.x, p->size * 2.0));
    Point2 y = addVector2(origin, multVector2(p->axis.y, p->size * 2.0));
    Point2 z = addVector2(origin, multVector2(p->inertia, p->size * 2 / MAX_SPEED_SHIP));
    Point2 dz = addVector2(p->shape[0], multVector2(p->moveLine, p->speed * 2 / MAX_SPEED_SHIP));

    if (!p->hideInertia)
        ImDrawList_AddLine(DRAW_LIST, (ImVec2){origin.x, origin.y}, (ImVec2){z.x, z.y}, CV_COL32(255, 0, 255, 200), 2.0f); //  inertia
    if (!p->hideSpeed)
        ImDrawList_AddLine(DRAW_LIST, (ImVec2){p->shape->x, p->shape->y}, (ImVec2){dz.x, dz.y}, CV_COL32(0, 0, 255, 200), 2.0f); // speed * moveline
    if (!p->hideAxis)
    {
        ImDrawList_AddLine(DRAW_LIST, (ImVec2){origin.x, origin.y}, (ImVec2){x.x, x.y}, CV_COL32(255, 0, 0, 200), 2.0f); // X axis aka targetline *2
        ImDrawList_AddLine(DRAW_LIST, (ImVec2){origin.x, origin.y}, (ImVec2){y.x, y.y}, CV_COL32(0, 255, 0, 200), 2.0f); //  Y axis
    }
    if (!p->hideSSphere)
        ImDrawList_AddCircle(DRAW_LIST, (ImVec2){origin.x, origin.y}, p->size, CV_COL32(255, 255, 255, 200), 50, 0.5f); // Surrounding sphere
    Point2 largeBody[3] = {p->shape[0], p->shape[3], p->shape[7]};
    for (int i = 0; i < 3; i++)
        PathLineTo_point2(largeBody[i]);
    ImDrawList_PathStroke(DRAW_LIST, CV_COL32(255, 255, 255, 200), ImDrawFlags_Closed | ImDrawFlags_RoundCornersAll, 1.5f);
}