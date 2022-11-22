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
}

void appUpdate(App *app)
{
    if (igIsKeyPressed(ImGuiKey_C, 0))
        app->debugMenu = !app->debugMenu;

    if (app->debugMenu)
    {
        ImGuiIO *io = igGetIO();
        Point2 mouse;
        igCheckbox("Move center with mouse e (right click)", &app->movePointE);
        // igCheckbox("Move center with mouse p (middle click)", &app->movePointP);
        igSliderInt("enemy id", &app->id, 0, MAX_ENEMY - 1, "%d", 0);
        igSliderInt("level", &g.level, 0, 30, "%d", 0);
        igSliderInt("lives p1", &p1.lives, 0, 30, "%d", 0);
        igText("Press 'x' to circle life status");
        igText("Press 'z' to circle menus");

        for (int i = 0; i < MAX_ENEMY; i++)
            if (en[i].status == ADULT)
            {
                ImDrawList_AddLine(igGetBackgroundDrawList_Nil(), (ImVec2){en[i].location.origin.x, en[i].location.origin.y}, (ImVec2){en[i].location.origin.x + 20 * en[i].location.x.x, en[i].location.origin.y + 20 * en[i].location.x.y}, CV_COL32(255, 0, 0, 200), 2.0f);
                ImDrawList_AddLine(igGetBackgroundDrawList_Nil(), (ImVec2){en[i].location.origin.x, en[i].location.origin.y}, (ImVec2){en[i].location.origin.x + 20 * en[i].location.y.x, en[i].location.origin.y + 20 * en[i].location.y.y}, CV_COL32(0, 255, 0, 200), 2.0f);
            }

        if (app->movePointE)
        {
            cvCoordsFromScreenSpace(io->MousePos.x, io->MousePos.y, &mouse.x, &mouse.y);
            igText("Mouse = { %.2f, %.2f }", mouse.x, mouse.y);
            g.enemy[app->id].location.origin.x = mouse.x;
            g.enemy[app->id].location.origin.y = mouse.y;
        }
        if (igIsMouseClicked(ImGuiMouseButton_Right, 0))
            app->movePointE = !app->movePointE;
        // if (igIsMouseClicked(ImGuiMouseButton_Middle, 0))
        //     app->movePointP = !app->movePointP;
        if (io->MouseWheel && g.enemy[app->id].size != FIXED)
            g.enemy[app->id].size = (g.enemy[app->id].size + 1) % 3;

        if (igIsKeyPressed(ImGuiKey_X, 0))
            g.enemy[app->id].status = (g.enemy[app->id].status + 1) % 3;
        if (igIsKeyPressed(ImGuiKey_Z, 0))
            g.menu = (g.menu + 1) % 5;
        if (igIsKeyPressed(ImGuiKey_P, 0))
            start_minelayer(&en[MAX_ENEMY - 1]);
    }

    //////////////////// LE GAME ITSELF /////////////////////////

    draw_menu(g.menu, app->font, g.score, g.level, g.is_p2, p1.lives, p2.lives);

    switch (g.menu)
    {
    case MAIN:
        g.cptDelta = 0.f;
        g.score = 0;
        g.level = 1;
        if (igIsKeyPressed(ImGuiKey_F, 0))
        {
            init_game(&p1, NULL, en, g.level);
            g.is_p2 = false;
            p1.lives = 3;
            g.menu = IN_GAME;
        }
        if (igIsKeyPressed(ImGuiKey_K, 0))
        {
            init_game(&p1, &p2, en, g.level);
            g.is_p2 = true;
            p1.lives = 3;
            p2.lives = 3;
            g.menu = IN_GAME;
        }
        if (igIsKeyPressed(ImGuiKey_Escape, 0))
            app->closeApp = true;
        break;

    case IN_GAME:
        if (is_any_enemy_alive(en, MAX_ENEMY - 1) || en[MAX_ENEMY - 1].status != DEAD)
        {
            g.cptDelta += igGetIO()->DeltaTime;
            update_game(en, &p1, &p2, igGetIO()->DeltaTime, g.cptDelta, MAX_ENEMY - 1);
            draw_all_enemy(en, MAX_ENEMY);
            if (p1.lives > 0)
                draw_player(p1, 1);
            if (g.is_p2 && (p2.lives > 0))
                draw_player(p2, 2);

            if (how_many_e_child(en, MAX_ENEMY - 1) == 0 && en[MAX_ENEMY - 1].status == CHILD)
                start_minelayer(&en[MAX_ENEMY - 1]);
            if (en[MAX_ENEMY - 1].status == ADULT)
                update_pos_any_enemy(&en[MAX_ENEMY - 1], p1.axis.origin);
        }
        else
        {
            g.level++;
            g.menu = SUCCESS;
        }

        if (p1.lives < 0 && (!g.is_p2 || p2.lives < 0))
            g.menu = GAMEOVER;

        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = PAUSE;
        break;

    case SUCCESS:
        if (igIsKeyPressed(ImGuiKey_Space, 0))
        {
            g.cptDelta = 0.f;
            if (g.is_p2)
                init_game(&p1, &p2, en, g.level);
            else
                init_game(&p1, NULL, en, g.level);
            g.menu = IN_GAME;
        }
        if (igIsKeyPressed(ImGuiKey_Escape, 0))
            g.menu = MAIN;
        break;

    case PAUSE:
        draw_all_enemy(en, MAX_ENEMY - 1);
        if (p1.lives > 0)
            draw_player(p1, 1);
        if (g.is_p2 && (p2.lives > 0))
            draw_player(p2, 2);
        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = IN_GAME;
        if (igIsKeyPressed(ImGuiKey_Escape, 0))
            g.menu = MAIN;
        break;

    case GAMEOVER:
        g.cptDelta = 0.f;
        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = MAIN;
        if (igIsKeyPressed(ImGuiKey_Escape, 0))
            app->closeApp = true;
        break;

    default:
        app->closeApp = true;
        break;
    }
}

void appShutdown(App *app)
{
    (void)app; // TOREMOVE: Silence unused parameter ‘app’ warning
}