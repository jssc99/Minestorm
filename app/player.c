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
    if (deltaTime && p->lives > 0)
    {
        // INPUTS
        if ((igIsKeyDown(ImGuiKey_D) && !p2) || (igIsKeyDown(ImGuiKey_J) && p2))
            turnleft_player(p, deltaTime);
        if ((igIsKeyDown(ImGuiKey_G) && !p2) || (igIsKeyDown(ImGuiKey_L) && p2))
            turnright_player(p, deltaTime);
        if ((igIsKeyDown(ImGuiKey_R) && !p2) || (igIsKeyDown(ImGuiKey_I) && p2))
            accelerate_player(p, deltaTime);
        if ((igIsKeyDown(ImGuiKey_F) && !p2) || (igIsKeyDown(ImGuiKey_K) && p2))
            fire_bullet(p, maxScreen);
        if (((igIsKeyPressed(ImGuiKey_E, 0) || (igIsKeyPressed(ImGuiKey_T, 0))) && !p2) || ((igIsKeyPressed(ImGuiKey_U, 0) || igIsKeyPressed(ImGuiKey_O, 0)) && p2))
            teleport_player(p, maxScreen, e);

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

// INPUTS

// Rotate the player
void rotate_player(Player *p, float angle)
{
    p->axis = rotateAxis2(p->axis, angle);
}

// Turn the player to the left
void turnleft_player(Player *p, float deltaTime)
{
    rotate_player(p, -M_PI * deltaTime);
    anim_right_thruster(*p);
}

// Turn the player to the right
void turnright_player(Player *p, float deltaTime)
{
    rotate_player(p, M_PI * deltaTime);
    anim_left_thruster(*p);
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
    anim_booster(*p);
}

// Teleport player at a random position, should check the collisions first
void teleport_player(Player *p, Point2 maxScreen, Enemy *e)
{
    if (p->tpcd >= 3)
    {
        Point2 newPos;
        srand(time(NULL));
        newPos.x = (int)(p->axis.origin.x + rand()) % (int)(maxScreen.x - 2 * p->size);
        newPos.y = (int)(p->axis.origin.y + rand()) % (int)(maxScreen.y - 2 * p->size);
        player_spawn_check(p, newPos, maxScreen, e); // Make sure we don't spawn on an enemy
        p->tpcd = 0;
    }
}

// Opens a window to debug Player
void debug_menu_player(Player *p, char playerNumber)
{
    char txtTmp[50];
    sprintf(txtTmp, "Player %d", playerNumber);
    igBegin(txtTmp, NULL, ImGuiWindowFlags_None);
    igText("Player%d", playerNumber);
    igText("Pos : (%f,%f)", p->axis.origin.x, p->axis.origin.y);
    igText("Lives = %d", p->lives);
    igText("Speed = %f", p->speed);
    if (igButton("hide axis", (ImVec2){0, 0}))
        p->hideAxis = !p->hideAxis;
    if (igButton("hide speed", (ImVec2){0, 0}))
        p->hideSpeed = !p->hideSpeed;
    if (igButton("hide inertia", (ImVec2){0, 0}))
        p->hideInertia = !p->hideInertia;
    if (igButton("hide sphere", (ImVec2){0, 0}))
        p->hideSSphere = !p->hideSSphere;
    igEnd();
}

// Fire a bullet
void fire_bullet(Player *p, Point2 maxScreen)
{
    if (p->firecd > 0.25) // 4 bullets/seconds max
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (p->bullets[i].lifespan == 0)
            {
                p->bullets[i] = init_bullet(*p, maxScreen);
                break;
            }
        }
        p->firecd = 0;
    }
}
/*
int play_sound(int argc, char** argv)
{
        ma_result result;
    ma_engine engine;

    if (argc < 2) {
        printf("No input file.");
        return -1;
    }

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return -1;
    }

    ma_engine_play_sound(&engine, argv[1], NULL);

    printf("Press Enter to quit...");
    getchar();

    ma_engine_uninit(&engine);

    return 0;
}*/