#include "enemy.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

void cvPathLineTo_point(Point2 p)
{
    cvPathLineTo(p.x, p.y);
}

void draw_fireball(Point2 center, int sides, float radius, unsigned int color)
{
    ImDrawList *drawList = igGetWindowDrawList();
    ImDrawList_AddCircleFilled(drawList, (ImVec2){center.x, center.y}, radius, color, sides); 
    
    /*
  for (int i = 0; i < sides; i++)
  {
       float angle = M_PI * 2.f * (float)i / (float)sides;
       Point2 point = {radius + center.x, center.y};
       point = rotatePoint2(center, point, angle);
       cvPathLineTo_point(point);
  }*/
    // cvPathFill(color);
    //  cvPathStroke(color, 1);
}

void draw_magnet_line(Point2 p, float size, float angle)
{
    Point2 top = {p.x, p.y - size * 1.6};
    cvPathLineTo_point(rotatePoint2(p, top, angle));
}

void draw_magnet(Point2 center, int sides, float radius, float angleOffset, float size, unsigned int color)
{
    sides++;
    for (int i = 0; i < sides; i++)
    {
        float angle = -M_PI * (float)i / (float)(sides - 1) + angleOffset;
        Point2 point = {-radius + center.x, center.y};
        point = rotatePoint2(center, point, angle);
        if (i == 0)
            draw_magnet_line(point, size, angleOffset);
        cvPathLineTo_point(point);
        if (i == (sides - 1))
            draw_magnet_line(point, size, angleOffset);
    }
    cvPathStroke(color, 0);
}

void draw_mine_body(Point2 center, float radius_big, float radius_small, float angleOffset, int tops)
{
    float angle = 0.f + angleOffset;
    for (int i = 0; i < tops; i++)
    {
        Point2 big = {center.x, center.y + radius_big};
        cvPathLineTo_point(rotatePoint2(center, big, angle));
        angle += M_PI / (float)tops;

        Point2 small = {center.x, center.y + radius_small};
        cvPathLineTo_point(rotatePoint2(center, small, angle));
        angle += M_PI / (float)tops;
    }
    cvPathStroke(CV_COL32_WHITE, 1);
}

void draw_floating_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   MAX_RADIUS_MINES * enemy.kSize,
                   FLOATING_SMALL_RADIUS * enemy.kSize,
                   enemy.angle, 3);
}

void draw_fireball_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   MAX_RADIUS_MINES * enemy.kSize,
                   FIREBALL_MINE_SMALL_RADIUS * enemy.kSize,
                   enemy.angle, 4);
    draw_fireball((Point2)enemy.location.origin, 12, enemy.kSize, CV_COL32_WHITE);
}

void draw_magnetic_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   MAX_RADIUS_MINES * enemy.kSize,
                   MAGNETIC_SMALL_RADIUS * enemy.kSize,
                   enemy.angle, 4);
    draw_magnet((Point2)enemy.location.origin, 12, 1.2 * enemy.kSize, enemy.angle, enemy.kSize, CV_COL32_WHITE);
}

void draw_magnet_fire_mine(Enemy enemy)
{
    float sizeTop = MAX_RADIUS_MINES * enemy.kSize;
    float size = MAGNET_FIRE_SMALL_RADIUS * enemy.kSize;
    Point2 center = {enemy.location.origin.x, enemy.location.origin.y};

    for (int i = 0; i < 4; i++)
    {
        float angle = M_PI * (float)i / 2.f + enemy.angle;
        Point2 p1 = {(-size) + center.x, center.y};
        cvPathLineTo_point(rotatePoint2(center, p1, angle));

        Point2 p2 = {(-sizeTop) + center.x, center.y};
        cvPathLineTo_point(rotatePoint2(center, p2, angle));
    }
    cvPathStroke(CV_COL32_WHITE, 1);

    draw_fireball((Point2)enemy.location.origin, 12, enemy.kSize, CV_COL32_WHITE);
    draw_magnet((Point2)enemy.location.origin, 12, 2.f * enemy.kSize, enemy.angle + M_PI / 6.f, enemy.kSize, CV_COL32_WHITE);
}

void draw_minelayer(Enemy enemy)
{
    Point2 center = enemy.location.origin;
    float size = enemy.kSize;

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
        cvPathLineTo_point(rotatePoint2(center, point[i], enemy.angle));

    cvPathStroke(CV_COL32_WHITE, 1);
}