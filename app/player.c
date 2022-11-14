#include "player.h"

bool DEBUG_PLAYER = 1;
const float MAX_SPEED_SHIP = 10;
const float ACCELERATION = 0.01;
const float DECELERATION = 0.01;
/*
void on_off(bool *b)
{
    if (b)
        b = false;
    else
        b = true;
}
*/

void debug_menu_player(Player *p, bool debugPlayer)
{
    igBegin("Player", &debugPlayer, ImGuiWindowFlags_None);
    igText("Lives = %d", p->lives);
    igText("Speed = %f", p->speed);
    if (DEBUG_PLAYER && igButton("Display axis", (ImVec2){0, 0}))
    {
        //   on_off(p->displayAxis);
        if (p->displayAxis)
            p->displayAxis = false;
        else
        {
            p->displayAxis = true;
        }
    }
    if (DEBUG_PLAYER && igButton("Display speed", (ImVec2){0, 0}))
    {
        if (p->displaySpeed)
            p->displaySpeed = false;
        else
            p->displaySpeed = true;
    }
    if (DEBUG_PLAYER && igButton("Display inertia", (ImVec2){0, 0}))
    {
        if (p->displayInertia)
            p->displayInertia = false;
        else
            p->displayInertia = true;
    }
    if (DEBUG_PLAYER && igButton("Display SS", (ImVec2){0, 0}))
    {
        if (p->displaySS)
            p->displaySS = false;
        else
            p->displaySS = true;
    }
    igEnd();
}
// Initialize Player at position(x,y) First time think about setting lives to 3.
Player player_init(Player p, float x, float y, float size)
{
    p.axis = (Axis2){{x, y},
                     {0, -size},
                     {size, 0}};
    p.inertia = (Vector2){0, 0};
    p.speed = 0;
    p.moveLine = p.axis.x;
    p.targetLine = p.moveLine;
    p.size = size;
    return p;
}

void draw_circle(Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color)
{
    Point2 point[sides];
    point[0].x = radius * sinf(angleOffset) + center.x;
    point[0].y = radius * cosf(angleOffset) + center.y;
    cvPathLineTo(point[0].x, point[0].y);

    float angle = M_PI * 2 / (float)sides;
    float baseX = point[0].x;
    float baseY = point[0].y;
    for (unsigned int i = 1; i < sides; i++)
    {
        float c = cosf(angle * i);
        float s = sinf(angle * i);
        point[i].x = (baseX - center.x) * c - (baseY - center.y) * s + center.x;
        point[i].y = (baseX - center.x) * s + (baseY - center.y) * c + center.y;
        cvPathLineTo(point[i].x, point[i].y);
    }
    cvPathStroke(color, 1);

    //return generate_SAT(point, sides);
}

// Draw the player
void draw_player(Player* player, unsigned int color)
{
    Player p = *player;
    Point2 origin = p.axis.origin;
    //player->sat = 
    draw_circle(origin, 3, p.size, getAngleVector2(p.axis.x, (Float2){0, 1}), color);
    if (DEBUG_PLAYER)
    {

        Point2 x = addVector2(origin, multVector2(p.axis.x, 2.0));
        Point2 y = addVector2(origin, multVector2(p.axis.y, 2.0));
        Point2 z = addVector2(origin, multVector2(p.inertia, p.size / 2));
        Point2 dz = addVector2(origin, multVector2(p.moveLine, p.speed / 2));
        if (p.displayInertia)
            cvAddLine(origin.x, origin.y, z.x, z.y, CV_COL32(255, 0, 255, 255)); //  inertia
        if (p.displaySpeed)
            cvAddLine(origin.x, origin.y, dz.x, dz.y, CV_COL32(0, 0, 255, 255)); // speed * moveline
        if (p.displayAxis)
        {
            cvAddLine(origin.x, origin.y, x.x, x.y, CV_COL32(255, 0, 0, 255)); // X axis aka targetline
            cvAddLine(origin.x, origin.y, y.x, y.y, CV_COL32(0, 255, 0, 255)); //  Y axis
        }
        if (p.displaySS)
        {
            draw_circle(origin, 50, p.size, 0, CV_COL32(255, 255, 255, 200));         // Surrounding sphere
            draw_circle((Float2){500, 400}, 50, 15, 0, CV_COL32(255, 255, 255, 200)); // Surrounding sphere mine
        }
    }
    // cvAddLine(p.axis.origin.x, p.axis.origin.y, p.axis.origin.x + 30 * p.axis.y.x, p.axis.origin.x + 30 * p.axis.y.y, CV_COL32(0, 0, 255, 255));
    //  cvAddLine(400, 300, 440, 300, CV_COL32(255, 0, 0, 255));
    //  cvAddLine(400, 300, 400, 330, CV_COL32(0, 0, 255, 255));
    //      AddTriangle(ImVec2(p.axis.x+sz*0.5f, p.axis.y), ImVec2(p.axis.x+sz,p.axis.y+sz-0.5f), ImVec2(p.axis.x,p.axis.y+sz-0.5f), color, th); x += sz+spacing;
}
// draw 1 bullet
void draw_bullet(Point2 center, unsigned int sides, float radius, unsigned int color)
{
    float angle = M_PI * 2.f / (float)sides;
    for (unsigned int i = 0; i < sides; i++)
    {
        Point2 point = {radius + center.x, center.y};
        point = rotatePoint2(center, point, angle * i);
        cvPathLineTo(point.x, point.y);
    }
    cvPathFill(color);
    // cvPathStroke(color, 1);
}

// Rotate the player
Player rotate_player(Player p, float angle)
{
    p.axis = rotateAxis2(p.axis, angle);
    return p;
}
// Check collision between a sphere and the screen and replace the object
void SS_collision_border_replace(Point2 *p, float size)
{
    if (p->x > 1000 - size)
        p->x = size;
    else if (p->x < size)
        p->x = 1000 - size;
    if (p->y > 800 - size)
        p->y = size;
    else if (p->y < size)
        p->y = 800 - size;
}

// Update the player each frame
Player update_player(Player p, float deltaTime)
{
    /*if (SS_collision_rectangle(p.axis.origin, p.size, 0, 0, 1000, 800))
        p = player_init(p, 500, 400, 30);*/
    SS_collision_border_replace(&p.axis.origin, p.size);
    p.targetLine = p.axis.x; // multVector2(addVector2(p.axis.x,p.axis.y), 0.5);
    // Deceleration
    p.speed = normVector2(p.inertia);
    p.inertia = multVector2(p.inertia, 1 - DECELERATION * deltaTime);
    // p.speed *= DECELERATION;
    p.axis = translateAxis2(p.axis, multVector2(p.inertia, deltaTime));
    // test collision mine
   /* if (SS_collision_SS(p.axis.origin, p.size, (Float2){500, 400}, 15))
    {
        p = player_init(p, 400, 300, p.size);
        p.lives--;
    }*/

    return p;
}

// Create a bullet
Bullet init_bullet(Player p)
{
    Bullet b;
    b.direction = p.targetLine;
    b.location = addVector2(p.axis.origin, p.targetLine);
    b.size = p.size / 10;
    b.lifespan = (800 / p.size) * 4 / 5;
    return b;
}

// Bullet evolution
Bullet update_bullet(Bullet b, float deltaTime)
{
    b.location = addVector2(b.location, multVector2(b.direction, deltaTime));
    SS_collision_border_replace(&b.location, b.size);
    b.lifespan -= deltaTime;
    if (b.lifespan < 0)
        b.lifespan = 0;
    //if (SS_collision_SS(b.location, b.size, (Point2){500, 400}, 15))
        b.lifespan = 0;
    return b;
}
// Turn the player to the left igIsKeyDown(ImGuiKey_D)
Player turnleft_player(Player p, float deltaTime)
{
    p = rotate_player(p, -M_PI / 24 * deltaTime);
    return p;
}

// Turn the player to the right igIsKeyDown(ImGuiKey_G)
Player turnright_player(Player p, float deltaTime)
{
    p = rotate_player(p, M_PI / 24 * deltaTime);
    return p;
}

Player accelerate_player(Player p, float deltaTime)
{
    if (p.speed < MAX_SPEED_SHIP)
    {
        p.speed += ACCELERATION * deltaTime;
    }
    else
        p.inertia = multVector2(p.inertia, MAX_SPEED_SHIP / p.speed);
    p.inertia = addVector2(p.inertia, multVector2(p.targetLine, ACCELERATION * deltaTime));
    p.moveLine = p.targetLine;

    return p;
}
/*
void counter_update(App *app, float frame)
{
    Counter->time_updated += frame;
    while (Counter->time_updated >= Counter->timer)
    {
        Counter->value++;
        Counter->time_updated -= Counter->timer;
    }
    Counter->timer = 1.f / Counter->freq;
}

void candy_counter(Counter *counter, const char *text)
{
    float frameTime = void counter_update(Counter *Counter, float frame)
{
    Counter->time_updated += frame;
    while (Counter->time_updated >= Counter->timer)
    {
        Counter->value++;
        Counter->time_updated -= Counter->timer;
    }
    Counter->timer = 1.f / Counter->freq;
}

void candy_counter(Counter *counter, const char *text)
{
    float frameTime = get_f
    pg_io_get_frame_time();
    counter_update(counter, frameTime);


    counter_update(counter, frameTime);
}
*/