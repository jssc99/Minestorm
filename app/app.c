#include "app.h"

#define en g.enemy
#define p1 g.player1
#define p2 g.player2

Game g;
void appInit(App *app)
{
    *app = (App){0};
    app->font = ImFontAtlas_AddFontFromFileTTF(igGetIO()->Fonts, "assets/Roboto-Regular.ttf", 100, NULL, NULL);
    app->backg = cvLoadTexture("assets/space.jpg");
    g = (Game){0};
}

void appUpdate(App *app)
{
    if (igIsKeyPressed(ImGuiKey_C, 0))
    {
        app->debugMenu = !app->debugMenu;
        DEBUG_PHYSIC = app->debugMenu;
    }

    // GAME BACKGROUND //
    ImDrawList_AddImage(igGetBackgroundDrawList_Nil(), app->backg.id,
                        (ImVec2){0.f, 0.f}, (ImVec2){700.f, 800.f},
                        (ImVec2){0.f, 0.f}, (ImVec2){1.f, 1.f},
                        igGetColorU32_Vec4((ImVec4){1.0f, 1.0f, 1.0f, 1.0f}));

    if (app->debugMenu)
    {
        ImGuiIO *io = igGetIO();
        Point2 mouse;

        igCheckbox("Move center of en with mouse (right click)", &app->movePointE);
        igSliderInt("enemy id", &app->id, 0, MAX_ENEMY - 1, "%d", 0);
        igSliderInt("level", &g.level, 0, 30, "%d", 0);
        igSliderInt("lives p1", &p1.lives, 0, 30, "%d", 0);
        igSliderInt("lives p2", &p2.lives, 0, 30, "%d", 0);
        igText("Press 'x' to circle life status");
        igText("Press 'z' to circle menus");
        igText("Press 'p' to spawn minelayer");
        igText("Use mousewheel to change size");

        for (int i = 0; i < MAX_ENEMY; i++)
            if (en[i].status == ADULT)
            {
                ImDrawList_AddLine(igGetBackgroundDrawList_Nil(), (ImVec2){en[i].location.origin.x, en[i].location.origin.y}, (ImVec2){en[i].location.origin.x + 20 * en[i].location.x.x, en[i].location.origin.y + 20 * en[i].location.x.y}, CV_COL32(255, 0, 0, 200), 2.0f);
                ImDrawList_AddLine(igGetBackgroundDrawList_Nil(), (ImVec2){en[i].location.origin.x, en[i].location.origin.y}, (ImVec2){en[i].location.origin.x + 20 * en[i].location.y.x, en[i].location.origin.y + 20 * en[i].location.y.y}, CV_COL32(0, 255, 0, 200), 2.0f);
            }

        if (igIsMouseClicked(ImGuiMouseButton_Right, 0))
            app->movePointE = !app->movePointE;
        if (app->movePointE)
        {
            cvCoordsFromScreenSpace(io->MousePos.x, io->MousePos.y, &mouse.x, &mouse.y);
            igText("Mouse = { %.2f, %.2f }", mouse.x, mouse.y);
            g.enemy[app->id].location.origin.x = mouse.x;
            g.enemy[app->id].location.origin.y = mouse.y;
        }
        if (io->MouseWheel && g.enemy[app->id].size != FIXED)
            g.enemy[app->id].size = (g.enemy[app->id].size + 1) % 3;

        if (igIsKeyPressed(ImGuiKey_X, 0))
            g.enemy[app->id].status = (g.enemy[app->id].status + 1) % 3;
        if (igIsKeyPressed(ImGuiKey_Z, 0))
            g.menu = (g.menu + 1) % 5;
        if (igIsKeyPressed(ImGuiKey_P, 0))
            en[MAX_ENEMY - 1].status = ADULT;

        {
            printf("[ en status: ");
            for (int i = 0; i < MAX_ENEMY; ++i)
                printf("%d%s", en[i].status, (i < MAX_ENEMY - 1) ? ", " : " ");
            printf("]\n");
        }
    }

    ///////////////////////////// LE GAME ITSELF ////////////////////////////////////

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
        else if (igIsKeyPressed(ImGuiKey_K, 0))
        {
            init_game(&p1, &p2, en, g.level);
            g.is_p2 = true;
            p1.lives = 3;
            p2.lives = 3;
            g.menu = IN_GAME;
        }
        else if (igIsKeyPressed(ImGuiKey_Escape, 0))
            app->closeApp = true;
        break;

    case IN_GAME:
        if (is_any_enemy_alive(en, MAX_ENEMY))
        {
            g.cptDelta += igGetIO()->DeltaTime;
            if (g.is_p2)
                update_game(en, &p1, &p2, igGetIO()->DeltaTime, g.cptDelta, &g.score);
            else
                update_game(en, &p1, NULL, igGetIO()->DeltaTime, g.cptDelta, &g.score);

            if (g.is_p2)
                draw_loop(en, &p1, &p2);
            else
                draw_loop(en, &p1, NULL);

            if (how_many_e_child(en, MAX_ENEMY - 1) == 0 && en[MAX_ENEMY - 1].status == CHILD)
                en[MAX_ENEMY - 1].status = ADULT;
            if (en[MAX_ENEMY - 1].status == ADULT)
            {
                update_pos_any_enemy(&en[MAX_ENEMY - 1], p1.axis.origin);
                bool found = 0;
                if ((int)en[MAX_ENEMY - 1].location.origin.y == 200 ||
                    (int)en[MAX_ENEMY - 1].location.origin.y == 400 ||
                    (int)en[MAX_ENEMY - 1].location.origin.y == 600)
                {
                    for (int i = 0; i < MAX_ENEMY - 1; i++)
                    {
                        if (!found && en[i].status == DEAD)
                        {
                            en[i] = init_enemy(en[MAX_ENEMY - 1].location.origin, FLOATING, SMALL);
                            en[i].status = ADULT;
                            found = true;
                        }
                    }
                }
            }
        }
        else
        {
            g.level++;
            g.menu = SUCCESS;
        }

        if (p1.lives <= 0 && (!g.is_p2 || p2.lives <= 0))
            g.menu = GAMEOVER;
        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = PAUSE;
        break;

    case SUCCESS:
        if (igIsKeyPressed(ImGuiKey_Space, 0))
        {
            g.cptDelta = 0.f;
            bullet_terminate(&p1, &p2);
            if (g.is_p2)
                init_game(&p1, &p2, en, g.level);
            else
                init_game(&p1, NULL, en, g.level);
            g.menu = IN_GAME;
        }
        else if (igIsKeyPressed(ImGuiKey_Escape, 0))
            g.menu = MAIN;
        break;

    case PAUSE:
        if (g.is_p2)
            draw_loop(en, &p1, &p2);
        else
            draw_loop(en, &p1, NULL);

        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = IN_GAME;
        else if (igIsKeyPressed(ImGuiKey_Escape, 0))
            g.menu = MAIN;
        break;

    case GAMEOVER:
        g.cptDelta = 0.f;
        update_pos_all_enemy(en, MAX_ENEMY, p1.axis.origin);
        draw_all_enemy(en, MAX_ENEMY);
        if (igIsKeyPressed(ImGuiKey_Space, 0))
            g.menu = MAIN;
        else if (igIsKeyPressed(ImGuiKey_Escape, 0))
            app->closeApp = true;
        break;

    default:
        app->closeApp = true;
        break;
    }
}

void appShutdown(App *app)
{
    cvUnloadTexture(app->backg);
    (void)app; // TOREMOVE: Silence unused parameter ‘app’ warning
}