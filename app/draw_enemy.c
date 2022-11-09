#include "enemy.h"

void cvPathLineTo_point(Point2 p)
{
    cvPathLineTo(p.x, p.y);
}

float get_size_multiplicator(enemySize size)
{
    switch (size)
    {
    case SMALL:
        return 2.f;
    case MEDIUM:
        return 4.f;
    case BIG:
        return 60.f;
    }
    return 0.f;
}

void draw_fireball(Point2 center, unsigned int sides, float radius, unsigned int color)
{
    float angle = M_PI * 2.f / (float)sides;
    for (int i = 0; i < sides; i++)
    {
        Point2 point = {radius + center.x, center.y};
        point = rotatePoint2(center, point, angle * i);
        cvPathLineTo_point(point);
    }
    cvPathFill(color);
    // cvPathStroke(color, 1);
}

void draw_magnet(Point2 center, unsigned int sides, float radius, float angleOffset, float size, unsigned int color)
{
    sides++;
    float angle = -M_PI / (float)(sides - 1);

    for (int i = 0; i < sides; i++)
    {
        float c = cosf(angle * i + angleOffset);
        float s = sinf(angle * i + angleOffset);
        Point2 point = {-radius * c + center.x, -radius * s + center.y};
        if (i == 0)
        {
            Point2 line = {point.x, point.y - size};
            line = rotatePoint2(point, line, angleOffset);
            cvPathLineTo_point(line);
        }
        cvPathLineTo_point(point);
        if (i == (sides - 1))
        {
            Point2 line = {point.x, point.y - size};
            line = rotatePoint2(point, line, angleOffset);
            cvPathLineTo_point(line);
        }
    }
    cvPathStroke(color, 0);
}

void draw_mine_body(Point2 center, float radius_big, float radius_small, float angleOffset, int tops)
{
    float cur_angle = 0.f + angleOffset;
    for (int i = 0; i < tops; i++)
    {
        cvPathLineTo(center.x + radius_big * cosf(cur_angle),
                     center.y + radius_big * sinf(cur_angle));
        cur_angle += M_PI / (float)tops;
        cvPathLineTo(center.x + radius_small * cosf(cur_angle),
                     center.y + radius_small * sinf(cur_angle));
        cur_angle += M_PI / (float)tops;
    }
    cvPathStroke(CV_COL32_WHITE, 1);
}

void draw_floating_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   6.f * get_size_multiplicator(enemy.size),
                   2.f * get_size_multiplicator(enemy.size),
                   enemy.angle, 3);
}

void draw_fireball_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   6.f * get_size_multiplicator(enemy.size),
                   3.f * get_size_multiplicator(enemy.size),
                   enemy.angle, 4);
    draw_fireball((Point2)enemy.location.origin, 12, get_size_multiplicator(enemy.size), CV_COL32_WHITE);
}

void draw_magnetic_mine(Enemy enemy)
{
    draw_mine_body(enemy.location.origin,
                   6.f * get_size_multiplicator(enemy.size),
                   2.5 * get_size_multiplicator(enemy.size),
                   enemy.angle, 4);
    // cvAddPoint(enemy.location.origin.x, enemy.location.origin.y, CV_COL32_WHITE);
    draw_magnet((Point2)enemy.location.origin, 12, 1.2 * get_size_multiplicator(enemy.size), enemy.angle, get_size_multiplicator(enemy.size), CV_COL32_WHITE);
}

void draw_magnet_fire_mine(Enemy enemy)
{
    Point2 point[4][2];
    float angle = M_PI / 2.f;
    float size = 6.f * get_size_multiplicator(enemy.size);
    float sizeTop = 2.f * get_size_multiplicator(enemy.size);

    for (int i = 0; i < 4; i++)
    {
        float c = cosf(angle * i + enemy.angle);
        float s = sinf(angle * i + enemy.angle);
        for (int j = 0; j < 2; j++)
        {
            point[i][j].x = (-size + (sizeTop * j)) * c + enemy.location.origin.x;
            point[i][j].y = (-size + (sizeTop * j)) * s + enemy.location.origin.y;
            cvPathLineTo_point(point[i][j]);
        }
    }
    cvPathStroke(CV_COL32_WHITE, 1);

    draw_fireball((Point2)enemy.location.origin, 12, 0.5 * get_size_multiplicator(enemy.size), CV_COL32_WHITE);
    draw_magnet((Point2)enemy.location.origin, 12, 2.f * get_size_multiplicator(enemy.size), enemy.angle + M_PI / 6.f, get_size_multiplicator(enemy.size), CV_COL32_WHITE);
}

void draw_minelayer(Enemy enemy)
{
    float x = enemy.location.origin.x;
    float y = enemy.location.origin.y;
    float kSize = get_size_multiplicator(MEDIUM);

    Point2 point[9] = {
        {x - 9.f * kSize, y + 2.f * kSize},
        {x - 11.f * kSize, y + 4.f * kSize},
        {x - 10.f * kSize, y},
        {x - 2.f * kSize, y},
        {x, y - 2.f * kSize},
        {x + 2.f * kSize, y},
        {x + 10.f * kSize, y},
        {x + 11.f * kSize, y + 4.f * kSize},
        {x + 9.f * kSize, y + 2.f * kSize}};

    for (int i = 0; i < 9; i++)
    {
        point[i] = rotatePoint2(enemy.location.origin, point[i], enemy.angle);
        cvPathLineTo_point(point[i]);
    }
    cvPathStroke(CV_COL32_WHITE, 1);
}