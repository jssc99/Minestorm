#include "player.h"

// Initialise Player at position(x,y) First time think about setting lives to 3.
void player_init(Player p, int x, int y)
{
    p.axis = (Axis2){(x, y), (1, 0), (0, 1)};
    p.moveLine = (Vector2){1, 1};
    p.targetLine = p.moveLine;
    p.speed = 0;
}

void draw_Polygon(Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color)
{
    Point2 point[sides];
    point[0].x = radius * sinf(angleOffset) + center.x;
    point[0].y = radius * cosf(angleOffset) + center.y;
    cvPathLineTo(point[0].x, point[0].y);

    float angle = M_PI * 2 / (float)sides;
    float baseX = point[0].x;
    float baseY = point[0].y;
    for (int i = 1; i < sides; i++)
    {
        float c = cosf(angle * i);
        float s = sinf(angle * i);
        point[i].x = (baseX - center.x) * c - (baseY - center.y) * s + center.x;
        point[i].y = (baseX - center.x) * s + (baseY - center.y) * c + center.y;
        cvPathLineTo(point[i].x, point[i].y);
    }
    cvPathStroke(color, 1);
}

// draws the player
void draw_player(Player p, unsigned int color, float sz) 
{
   draw_Polygon(p.axis.origin, 3, normVector2(addVector2(p.axis.x,p.axis.y)),0,color);
//    AddTriangle(ImVec2(p.axis.x+sz*0.5f, p.axis.y), ImVec2(p.axis.x+sz,p.axis.y+sz-0.5f), ImVec2(p.axis.x,p.axis.y+sz-0.5f), color, th); x += sz+spacing; 
    return;
}