#include "player.h"

bool DEBUG_PLAYER = 1;

#define drawList igGetBackgroundDrawList_Nil()
// static ImU32 color;

void debug_menu_player(Player *p, bool debugMenu)
{
    igBegin("Player", &debugMenu, ImGuiWindowFlags_None);
    igText("Pos : (%f,%f)", p->axis.origin.x, p->axis.origin.y);
    igText("Lives = %d", p->lives);
    igText("Speed = %f", p->speed);
    if (DEBUG_PLAYER && igButton("Display axis", (ImVec2){0, 0}))
        p->displayAxis = !p->displayAxis;
    if (DEBUG_PLAYER && igButton("Display speed", (ImVec2){0, 0}))
        p->displaySpeed = !p->displaySpeed;
    if (DEBUG_PLAYER && igButton("Display inertia", (ImVec2){0, 0}))
        p->displayInertia = !p->displayInertia;
    if (DEBUG_PLAYER && igButton("Display sphere", (ImVec2){0, 0}))
        p->displaySSphere = !p->displaySSphere;
    if (DEBUG_PLAYER && igButton("Collision Tests", (ImVec2){0, 0}))
        p->collisionTests = !p->collisionTests;

    igEnd();
}
// Spawn player at Point (x,y)
void player_spawn(Player *p, float x, float y)
{
    p->axis = (Axis2){{x, y},
                      {0, -1},
                      {1, 0}};
    p->inertia = (Vector2){0, 0};
    p->speed = 0;
    p->moveLine = multVector2(p->axis.x, p->size);
    p->targetLine = p->moveLine;
}
// Initialize Player at position(x,y) First time think about setting lives to 3.
Player player_init(float x, float y, float size)
{
    Player p = {0};
    p.lives = 3;
    p.size = size;
    player_spawn(&p, x, y);
    return p;
}

// Set the shape of the player
void init_points_player(Player *p)
{
    float x = p->axis.origin.x;
    float y = p->axis.origin.y;
    Point2 origin = p->axis.origin;
    Vector2 axeY = p->axis.x; // initialize Rocket with arrow up
    Vector2 axeX = p->axis.y;
    Point2 point[10] = {
        translatePoint2(origin, multVector2(axeY, 25.f)),                                        // Head   {0,25}
        translatePoint2(origin, multVector2(axeX, -5.f)),                                        //        {0,-5}
        translatePoint2(origin, multVector2(axeX, -8.f)),                                        //        {0,-8}
        translatePoint2(origin, addVector2(multVector2(axeX, -15.f), multVector2(axeY, -20.f))), //        {-15,-20}
        translatePoint2(origin, addVector2(multVector2(axeX, -4.2f), multVector2(axeY, -07.f))), //        {-4.-2,7}
        translatePoint2(origin, multVector2(axeY, -12.f)),                                       // Tail   {0,-12}
        translatePoint2(origin, addVector2(multVector2(axeX, +4.2f), multVector2(axeY, -07.f))), //        {4.2,7}
        translatePoint2(origin, addVector2(multVector2(axeX, +15.f), multVector2(axeY, -20.f))), //        {15,20}
        translatePoint2(origin, multVector2(axeX, 8.f)),                                         //        {0, 8}
        translatePoint2(origin, multVector2(axeX, 5.f))};                                        //        {0, 5}

    for (int i = 0; i < 10; i++)
        p->shape[i] = point[i];
}

// Collision with enemy

// Rotate the player
void rotate_player(Player *p, float angle)
{
    p->axis = rotateAxis2(p->axis, angle);
}

// Fire a bullet
void fire_bullet(Player *p, float deltaTime, Point2 maxScreen)
{
    if (p->firecd > 0.25)
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (p->bullets[i].lifespan == 0)
            {
                p->bullets[i] = init_bullet(*p);
                break;
            }
        }
        p->firecd = 0;
    }
}
// Update the player each frame
void update_player(Player *p, float deltaTime, Point2 maxScreen, bool p2)
{
    if (deltaTime)

        if (p->lives >= 0)
        {
            // INPUTS
            if ((igIsKeyDown(ImGuiKey_D) && !p2) || (igIsKeyDown(ImGuiKey_J) && p2))
                turnleft_player(p, deltaTime);
            if ((igIsKeyDown(ImGuiKey_G) && !p2) || (igIsKeyDown(ImGuiKey_L) && p2))
                turnright_player(p, deltaTime);
            if ((igIsKeyDown(ImGuiKey_R) && !p2) || (igIsKeyDown(ImGuiKey_I) && p2))
                accelerate_player(p, deltaTime);
            if ((igIsKeyDown(ImGuiKey_F) && !p2) || (igIsKeyDown(ImGuiKey_K) && p2))
                fire_bullet(p, deltaTime, maxScreen);
            if (((igIsKeyPressed(ImGuiKey_E, 0) || (igIsKeyPressed(ImGuiKey_T, 0))) && !p2) || ((igIsKeyPressed(ImGuiKey_U, 0) || (igIsKeyPressed(ImGuiKey_O, 0))) && p2))
                teleport_player(p, maxScreen);

            // Collisions
            poly_collision_border_replace(p->shape, &p->axis.origin, 10, p->size, maxScreen);

            // test collision mine
            if (sphere_collision_sphere(p->axis.origin, p->size, (Float2){500, 400}, 15))
            {
                player_spawn(p, 400, 300);
                p->lives--;
            }
            else
            {
                // p->targetLine = p->axis.x; // multVector2(addVector2(p->axis.x,p->axis.y), 0.5);
                p->moveLine = multVector2(p->axis.x, p->size);
                p->targetLine = p->moveLine;
                // Deceleration
                p->speed = normVector2(p->inertia);
                p->inertia = multVector2(p->inertia, 1 - DECELERATION * deltaTime);
                // p->speed *= DECELERATION;
                // Displacement
                p->axis = translateAxis2(p->axis, multVector2(p->inertia, deltaTime));
            }
            update_bullet(p, deltaTime, maxScreen);
            p->firecd += deltaTime;
            p->tpcd += deltaTime;
            init_points_player(p);
        }
}

// Create a bullet
Bullet init_bullet(Player p)
{
    Bullet b;
    b.direction = multVector2(p.targetLine, 1 + (p.speed / MAX_SPEED_SHIP)); // Keeps the ship's speed
    b.location = addVector2(p.axis.origin, p.targetLine);
    b.size = p.size / 10;
    b.lifespan = (800 / p.size) * 4 / 5;
    return b;
}

// Draw ALL the bullets of a player
void update_bullet(Player *p, float deltaTime, Point2 maxScreen)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (p->bullets[i].lifespan)
        {
            update_one_bullet(&p->bullets[i], deltaTime, maxScreen);
            draw_bullet(p->bullets[i].location, p->bullets[i].size, CV_COL32(255, 255 / MAX_BULLETS * i, 0, 255));
        }
    }
}
// Bullet evolution
void update_one_bullet(Bullet *b, float deltaTime, Point2 maxScreen)
// TODO: void update_bullet(Bullet* b, float deltaTime)
{
    b->location = addVector2(b->location, multVector2(b->direction, 30 * deltaTime));
    sphere_collision_border_replace(&b->location, b->size, maxScreen);
    b->lifespan -= deltaTime * 30;
    if (b->lifespan < 0)
        b->lifespan = 0;
    if (sphere_collision_sphere(b->location, b->size, (Point2){500, 400}, 15))
        b->lifespan = 0;
}
// Turn the player to the left igIsKeyDown(ImGuiKey_D)
void turnleft_player(Player *p, float deltaTime)
{
    rotate_player(p, -M_PI * deltaTime);
}

// Turn the player to the right igIsKeyDown(ImGuiKey_G)
void turnright_player(Player *p, float deltaTime)
{
    rotate_player(p, M_PI * deltaTime);
}

// Add ACCELERATION to speed
void accelerate_player(Player *p, float deltaTime)
{
    if (p->speed < MAX_SPEED_SHIP)
    {
        p->speed += ACCELERATION * deltaTime;
    }
    else
        p->inertia = multVector2(p->inertia, MAX_SPEED_SHIP / p->speed);
    p->inertia = addVector2(p->inertia, multVector2(p->targetLine, ACCELERATION * deltaTime));
    p->moveLine = p->targetLine;
}
// Teleport player at a random position, should check the collisions first
void teleport_player(Player *p, Point2 maxScreen)
{
    if (p->tpcd >= 3)
    {
        // while collision
        srand(time(NULL)); // i++ ?
        int newPosx = (int)(p->axis.origin.x + rand()) % (int)(maxScreen.x - 2 * p->size);
        int newPosY = (int)(p->axis.origin.y + rand()) % (int)(maxScreen.y - 2 * p->size);
        player_spawn(p, p->size + newPosx, p->size + newPosY);
        p->tpcd = 0;
    }
}

// Draw a circle or a polygon with Canvas (obsolete)
void draw_circle(Point2 *cBox, Point2 center, unsigned int sides, float radius, float angleOffset, unsigned int color)
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

    if (cBox)
    {
        for (unsigned int i = 0; i < sides; i++)
            cBox[i] = point[i];
    }
}

void test_collision(Player player1, ImVec2 mousePos)
{
    // Rectangle collision
    // bool collision = sphere_collision_rectangle((Point2){mousePos.x, mousePos.y}, 2, 460, 360, 540, 440);
    Point2 carre[4];
    draw_circle(&carre, (Point2){500, 400}, 4, 40 * sqrtf(2), M_PI / 4, CV_COL32_WHITE);

    // Sat collision
    /*
    Point2 quad[4] = {{390.f, 350.f}, {410.f, 440.f}, {520.f, 440.f}, {540.f, 380.f}};
    Point2 triangle[3] = {{520.f, 390.f}, {480.f, 390.f}, {500.f, 430.f}};
    Point2 poly[6] = {{480.f, 320.f}, {410.f, 350.f}, {390.f, 400.f}, {410.f, 440.f}, {520.f, 470.f}, {540.f, 380.f}};
    for (int i = 0; i < 6; i++)
    {
        poly[i] = rotatePoint2((Point2){500, 400}, poly[i], M_PI / 6);
    }

    Point2 poly_[6];
    for (int i = 0; i < 6; i++)
        poly_[i] = poly[6 - 1 - i];
    Point2 poly_m[6];
    for (int i = 0; i < 6; i++)
    {
        poly_m[i].x = poly[i].x + mousePos.x - 500.f;
        poly_m[i].y = poly[i].y + mousePos.y - 400.f;
    }
    for (int i = 0; i < 6; i++)
    {
        cvAddLine(poly_m[i].x, poly_m[i].y, poly_m[(i + 1) % 6].x, poly_m[(i + 1) % 6].y, CV_COL32(255, 0, 0, 255));
    }
    draw_circle(NULL, circumcenterTriangle(triangle[1], triangle[2], triangle[0]), 50, 4 * 3 * sqrtf(3), 0, CV_COL32(0, 0, 255, 255));
        for (int i = 0; i < 4; i++)
            {
                cvAddLine(quad[i].x, quad[i].y, quad[(i + 1) % 4].x, quad[(i + 1) % 4].y, CV_COL32(255, 0, 0, 255));
            }

       bool collision = sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, quad, 4);

    for (int i = 0; i < 3; i++)
    {
        cvAddLine(triangle[i].x, triangle[i].y, triangle[(i + 1) % 3].x, triangle[(i + 1) % 3].y, CV_COL32(255, 0, 0, 255));
    }
    bool collision = sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, triangle, 3);

    cvAddLine(poly[0].x, poly[0].y, poly[(0 + 1) % 6].x, poly[(0 + 1) % 6].y, CV_COL32(255, 0, 255, 255));
    for (int i = 1; i < 6; i++)
    {
        cvAddLine(poly[i].x, poly[i].y, poly[(i + 1) % 6].x, poly[(i + 1) % 6].y, CV_COL32(255, 0, 0, 255));
    }
    Point2 largeBody[3] = {player1.shape[0], player1.shape[3], player1.shape[7]}; // exclude 2 points :(
    Point2 arrow[3] = {player1.shape[0], player1.shape[1], player1.shape[9]};
    Point2 leftWing[3] = {player1.axis.origin, player1.shape[2], player1.shape[3]};
    Point2 rightWing[3] = {player1.axis.origin, player1.shape[7], player1.shape[8]};
    Point2 tail[3] = {player1.shape[1], player1.shape[5], player1.shape[8]};
    */
    Point2 LARGEBODY(player1);
    Point2 ARROW(player1);
    Point2 LEFTWING(player1);
    Point2 RIGHTWING(player1);
    Point2 TAIL(player1);
    /*
    bool collision = (sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, arrow, 3)        //
                      || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, leftWing, 3)  //
                      || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, rightWing, 3) //
                      || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, tail, 3));
                      */
    // bool collision = SAT_collision_SAT(poly_m, 6, poly, 6);
    // bool collision = sphere_collision_rectangle((Point2){mousePos.x, mousePos.y}, 20, 0, 0, 700, 800);
     bool collision = sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, carre, 4);
    /*for (int i = 0; i < 3; i++)
    {
        printf("Points [%d], = (%f,%f)\n", i, player1.shape[i].x, player1.shape[i].y);
    }
    */
    draw_circle(NULL, (Point2){mousePos.x, mousePos.y}, 50, 2, 0, CV_COL32((255 * collision), (!collision * 255), 0, 255));
    // printf("min = %f, max = %f, x = %f, y = %f\n", player1.sat->min, player1.sat->max, player1.sat->normal.x, player1.sat->normal.y);
}