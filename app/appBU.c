#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS

#include <cimgui.h>

#include <canvas.h>
#include "app.h"

#define e game.enemy[0]
#define p1 game.player1
#define p2 game.player2
#define b game.bullets

const bool MULTIP = true;
Game game;

void appInit(App *app)
{
    ImGuiIO *io = igGetIO();
    int width = io->DisplaySize.x;
    int height = io->DisplaySize.y;
    *app = (App){0};

    // create_minefield(&e, 1, width, height);
    p1 = player_init(300, 400, 25);
    p2 = player_init(500, 400, 25);
    create_minefield(&e, 1, width, height);
    e = init_enemy(e.location.origin, FLOATING, BIG);
    e.status = ADULT;
    
    game.menu = IN_GAME;
}

/*
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
*/

void appUpdate(App *app)
{
    ImGuiIO *io = igGetIO();
    int width = io->DisplaySize.x;
    int height = io->DisplaySize.y;
    float deltaTime = 0 ;
    if (game.menu == IN_GAME)
        deltaTime += io->DeltaTime;
    /*
    cvSetCoordinateSystemFromScreenSpace(
        0, 0,      // origin
        100.f, 0.f, // right
        0.f, 100.f  // top
    );0*/

    // tentative de faire des ennemis
    //  update_pos_any_enemy(e);

    // draw_poly(e.points, e.nbPoints);
    update_pos_any_enemy(&e, game.player1.axis.origin);

    // Player redefinition
    if (p1.lives >= 0)
    {
        debug_menu_player(&p1, true);
        if (game.menu == IN_GAME)
            update_player(&p1, deltaTime, (Point2){width, height}, false);
        draw_player(p1, 1);
        if(app->debugMenu)
        draw_debug_player(&p1);

        if (p1.collisionTests)
            test_collision(p1, p2, io->MousePos);
    }
    if (MULTIP && p2.lives >= 0)
    {
        debug_menu_player(&p2, true);
        if (game.menu == IN_GAME)
            update_player(&p2, deltaTime, (Point2){width, height}, true);
        draw_player(p2, 2);
        if(app->debugMenu)
        draw_debug_player(&p2);
    }
    if (igIsKeyPressed(ImGuiKey_Space, 0))
    {
        if (game.menu == PAUSE)
            game.menu = IN_GAME;
        else if (game.menu == IN_GAME)
            game.menu = PAUSE;
    }
    if(igIsKeyPressed(ImGuiKey_C, 0))
    {
        app->debugMenu = !app->debugMenu;
    }
}

void appShutdown(App *app)
{
    (void)app; // TOREMOVE: Silence unused parameter ‘app’ warning
}