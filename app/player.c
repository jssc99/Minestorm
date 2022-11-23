#include "player.h"
#include "enemy.h"


// Initialize Player at position(x,y) First time think about setting lives to 3.
Player player_init(float x, float y, float size)
{
    Player p = {0};
    p.lives = 3;
    p.size = size;
    p.spawnPoint = (Point2){x, y};
    player_spawn(&p, x, y);
    return p;
}

// Spawn player at Point (x,y)
void player_spawn(Player *p, float x, float y)
{
    p->axis = (Axis2){{x, y},
                      {0, -p->size / 25.f}, // 25 px is the base size of the ship
                      {p->size / 25.f, 0}};
    p->inertia = (Vector2){0, 0};
    p->speed = 0;
    p->moveLine = p->axis.x;
    p->targetLine = p->moveLine;
}

// Check collisions before spawn
void player_spawn_check(Player *p, Point2 newLocation, Point2 maxScreen, Enemy *e)
{
    srand(time(NULL));
    bool collision = false;
    do
    {
        for (int i = 0; i < MAX_ENEMY; i++)
            if (e[i].status == ADULT)
            {
                collision = sphere_collision_sphere(newLocation, 2 * p->size, e[i].location.origin, get_max_size(e[i].size, e[i].type));
                break;
            }
        if (!collision)
        {
            player_spawn(p, newLocation.x, newLocation.y);
            return;
        }
        newLocation.x = (int)(p->axis.origin.x + rand()) % (int)(maxScreen.x - 2 * p->size) + p->size;
        newLocation.y = (int)(p->axis.origin.y + rand()) % (int)(maxScreen.y - 2 * p->size) + p->size;
    } while (collision);
}

// Set the shape of the player
void init_points_player(Player *p)
{
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
        translatePoint2(origin, multVector2(axeX, 8.f)),                                         //        {0,8}
        translatePoint2(origin, multVector2(axeX, 5.f))};                                        //        {0, 5}

    for (int i = 0; i < 10; i++)
        p->shape[i] = point[i];
}

// Update the player each frame
void update_player(Player *p, float deltaTime, Point2 maxScreen, bool p2, Enemy *e)
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
            if (((igIsKeyPressed(ImGuiKey_E, 0) || (igIsKeyPressed(ImGuiKey_T, 0))) && !p2) || (((igIsKeyPressed(ImGuiKey_U, 0)) || (igIsKeyPressed(ImGuiKey_O, 0)) && p2)))
                teleport_player(p, maxScreen);


            // Collisions
            poly_collision_border_replace(p->shape, &p->axis.origin, 10, p->size, maxScreen);

        p->moveLine = multVector2(p->axis.x, p->size);
        p->targetLine = p->moveLine;
        // Deceleration
        p->speed = normVector2(p->inertia);
        p->inertia = multVector2(p->inertia, 1 - DECELERATION * deltaTime);
        // Displacement
        p->axis = translateAxis2(p->axis, multVector2(p->inertia, deltaTime));
    }
    update_bullet(p, deltaTime, maxScreen);
    p->firecd += deltaTime;
    p->tpcd += deltaTime;
    init_points_player(p);
}

//INPUTS

// Rotate the player
void rotate_player(Player *p, float angle)
{
    p->axis = rotateAxis2(p->axis, angle);
}

// Turn the player to the left
void turnleft_player(Player *p, float deltaTime)
{
    rotate_player(p, -M_PI * deltaTime);
}

// Turn the player to the right
void turnright_player(Player *p, float deltaTime)
{
    rotate_player(p, M_PI * deltaTime);
}

// Add ACCELERATION to speed
void accelerate_player(Player *p, float deltaTime)
{
    if (p->speed < MAX_SPEED_SHIP) // Never exceed MAX_SPEED
    {
        p->speed += ACCELERATION * deltaTime;
    }
    else
        p->inertia = multVector2(p->inertia, MAX_SPEED_SHIP / p->speed);                       // Re-Calculate to keep Max Speed
    p->inertia = addVector2(p->inertia, multVector2(p->targetLine, ACCELERATION * deltaTime)); // Update inertia
    p->moveLine = p->targetLine;
}

// Teleport player at a random position, should check the collisions first
void teleport_player(Player *p, Point2 maxScreen, Enemy *e)
{
    if (p->tpcd >= 3)
    {
        // while collision
        srand(time(NULL)); // i++ ?
        int newPosx = (int)(p->axis.origin.x + rand()) % (int)(maxScreen.x - 2*p->size);
        int newPosY = (int)(p->axis.origin.y + rand()) % (int)(maxScreen.y - 2*p->size);
        player_spawn(p, p->size + newPosx, p->size + newPosY);
        p->tpcd = 0;
    }
}

// Opens a window to debug Player
void debug_menu_player(Player *p, int playerNumber)
{
    igBegin("Player", 0, ImGuiWindowFlags_None);
    igText("Player%d", playerNumber);
    igText("Pos : (%f,%f)", p->axis.origin.x, p->axis.origin.y);
    igText("Lives = %d", p->lives);
    igText("Speed = %f", p->speed);
    if (igButton("Display axis", (ImVec2){0, 0}))
        p->hideAxis = !p->hideAxis;
    if (igButton("Display speed", (ImVec2){0, 0}))
        p->hideSpeed = !p->hideSpeed;
    if (igButton("Display inertia", (ImVec2){0, 0}))
        p->hideInertia = !p->hideInertia;
    if (igButton("Display sphere", (ImVec2){0, 0}))
        p->hideSSphere = !p->hideSSphere;
    if (igButton("Collision Tests", (ImVec2){0, 0}))
        p->hideCollisionBox = !p->hideCollisionBox;
    igEnd();
}

// Fire a bullet
void fire_bullet(Player *p, Point2 maxScreen)
{
    // Rectangle collision
    // bool collision = sphere_collision_rectangle((Point2){mousePos.x, mousePos.y}, 2, 460, 360, 540, 440);
    draw_circle(NULL, (Point2){500, 400}, 4, 40 * sqrtf(2), M_PI / 4, CV_COL32_WHITE);

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
                p->bullets[i] = init_bullet(*p, maxScreen);
                break;
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
    bool collision = (sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, arrow, 3)        //
                      || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, leftWing, 3)  //
                      || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, rightWing, 3) //
                      || sphere_collision_SAT((Point2){mousePos.x, mousePos.y}, 2, tail, 3));
    // bool collision = SAT_collision_SAT(poly_m, 6, poly, 6);
    // bool collision = sphere_collision_rectangle((Point2){mousePos.x, mousePos.y}, 20, 0, 0, 700, 800);
    /*for (int i = 0; i < 3; i++)
    {
        printf("Points [%d], = (%f,%f)\n", i, player1.shape[i].x, player1.shape[i].y);
    }
}
