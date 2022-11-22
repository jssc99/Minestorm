#include "app.h"

#define en g.enemy
#define p1 g.player1
#define p2 g.player2

Game g;

void appInit(App *app)
{
    *app = (App){0};
    app->font = ImFontAtlas_AddFontFromFileTTF(igGetIO()->Fonts, "assets/Roboto-Regular.ttf", 100, NULL, NULL);
    g = (Game){0};
    g.level = 1;
    p1.lives = 3;
    p2.lives = 0;
    g.is_p2 = true;
}

void appUpdate(App *app)
{
    ImGuiIO *io = igGetIO();
    Point2 mouse;
    if (app->debugMenu)
    {
        igCheckbox("Move center with mouse e", &app->movePointE);
        igCheckbox("Move center with mouse p", &app->movePointP);
        igCheckbox("Rotate the figure(s)", &app->rotate);
        igSliderAngle("Angle", &app->angle, 0.f, 360.f, "%.2f", 0);
        igSliderInt("enemy id", &app->id, 0, MAX_ENEMY - 1, "%d", 0);
    }
    if (app->movePointE)
    {
        cvCoordsFromScreenSpace(io->MousePos.x, io->MousePos.y, &mouse.x, &mouse.y);
        if (app->debugMenu)
            igText("Mouse = { %.2f, %.2f }", mouse.x, mouse.y);
        g.enemy[app->id].location.origin.x = mouse.x;
        g.enemy[app->id].location.origin.y = mouse.y;
    }
    if (igIsMouseClicked(ImGuiMouseButton_Right, 0))
        app->movePointE = !app->movePointE;
    if (igIsMouseClicked(ImGuiMouseButton_Middle, 0))
        app->movePointP = !app->movePointP;
    if (io->MouseWheel && g.enemy[app->id].size != FIXED)
        g.enemy[app->id].size = (g.enemy[app->id].size + 1) % 3;

    if (app->rotate)
        app->angle += 0.01;
    if (app->angle > 2.f * M_PI + 0.0001)
        app->angle = 0.f;
    g.enemy[app->id].angle = app->angle;

    if (igIsKeyPressed(ImGuiKey_C, 0))
        app->debugMenu = !app->debugMenu;
    if (igIsKeyPressed(ImGuiKey_X, 0))
        g.enemy[app->id].status = (g.enemy[app->id].status + 1) % 3;
    if (igIsKeyPressed(ImGuiKey_Z, 0))
        g.menu = (g.menu + 1) % 4;

    //////////////////// LE GAME ITSELF /////////////////////////

    draw_menu(g.menu, app->font, g.score, g.is_p2, p1.lives, p2.lives);

    switch (g.menu)
    {
    case MAIN:
        if (igIsKeyPressed(ImGuiKey_F, 0))
        {
            init_game(&(p1), NULL, en, g.level);
            g.is_p2 = false;
            g.menu = IN_GAME;
            //TEST
            //for (int i = 0; i < MAX_ENEMY; i++)
            //    en[i].status = ADULT;
            //update_pos_all_enemy(en, MAX_ENEMY - 1, (Point2){350,400});
                //END TEST
        }
        if (igIsKeyPressed(ImGuiKey_K, 0))
        {
            init_game(&(p1), &(p2), en, g.level);
            p2.lives = 3;
            g.menu = IN_GAME;
        }
        if (igIsKeyPressed(ImGuiKey_Escape, 0))
            app->closeGame = true;
        break;

    case IN_GAME:
        if (is_any_enemy_alive(en, MAX_ENEMY - 1))
        {
            game_update(en, &p1, &p2, igGetIO()->DeltaTime, MAX_ENEMY - 1);
            draw_all_enemy(en, MAX_ENEMY - 1);
            if (p1.lives)
                draw_player(p1, 1);
            if (g.is_p2 && p2.lives)
                draw_player(p2, 2);
                //TEST COLLISION
           // for (int i = 0; i < MAX_ENEMY; i++)
           // test_collision(p1, p2, io->MousePos, en[0]);
                //end TEST
        }

        if (!p1.lives && (!g.is_p2 || !p2.lives))
            g.menu = GAMEOVER;
        if (!is_any_enemy_alive(en, MAX_ENEMY - 1))
            // GREAT SUCCES !

        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = PAUSE;
        break;

    case PAUSE:
        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = IN_GAME;
        if (igIsKeyPressed(ImGuiKey_Escape, 0))
            app->closeGame = true;
        break;

    case GAMEOVER:
        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = MAIN;
        if (igIsKeyPressed(ImGuiKey_Escape, 0))
            app->closeGame = true;
        break;

    default:
        app->closeGame = true;
        break;
    }
}

void appShutdown(App *app)
{
    (void)app; // TOREMOVE: Silence unused parameter ‘app’ warning
}