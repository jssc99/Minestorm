#include "menu.h"

static ImDrawList *drawList;
static ImU32 color;

void draw_play_menu()
{
    color = igGetColorU32_Vec4(((ImVec4){0.f, 0.8f, 1.0f, 1.0f}));
    ImDrawList_AddRectFilled(drawList, (ImVec2){20.f, 120.f}, (ImVec2){160.f, 160.f}, color, 10.f, ImDrawFlags_RoundCornersAll);
    color = igGetColorU32_Vec4(((ImVec4){0.f, 1.0f, 0.1f, 1.0f}));
    ImDrawList_AddRectFilled(drawList, (ImVec2){590.f, 120.f}, (ImVec2){730.f, 160.f}, color, 10.f, ImDrawFlags_RoundCornersAll);

    color = igGetColorU32_Vec4(((ImVec4){1.f, 1.f, 1.f, 1.f}));
    // ImFontAtlas *atlas = ImFontAtlas_ImFontAtlas();
    // ImFont *font = ImFontAtlas_AddFontFromFileTTF(atlas, "assets/Roboto-Regular.ttf", 48.f, NULL, NULL);
    // ImDrawList_AddText_FontPtr(drawList, font, 2.f, (ImVec2){25.f, 25.f}, color, "Player One", NULL, 10.f, NULL);
    ImDrawList_AddText_Vec2(drawList, (ImVec2){25.f, 125.f}, color, "Player One", NULL);
    {
        ImDrawList_AddText_Vec2(drawList, (ImVec2){25.f, 165.f}, color, "Rotate: 'D' & 'G'", NULL);
        ImDrawList_AddText_Vec2(drawList, (ImVec2){25.f, 180.f}, color, "Move: 'R'", NULL);
        ImDrawList_AddText_Vec2(drawList, (ImVec2){25.f, 195.f}, color, "Shoot: 'F'", NULL);
        ImDrawList_AddText_Vec2(drawList, (ImVec2){25.f, 210.f}, color, "Teleport: 'E' or 'T'", NULL);
    }

    ImDrawList_AddText_Vec2(drawList, (ImVec2){595.f, 125.f}, color, "Player Two", NULL);
    {
        ImDrawList_AddText_Vec2(drawList, (ImVec2){595.f, 165.f}, color, "Rotate: 'J' & 'L'", NULL);
        ImDrawList_AddText_Vec2(drawList, (ImVec2){595.f, 180.f}, color, "Move: 'I'", NULL);
        ImDrawList_AddText_Vec2(drawList, (ImVec2){595.f, 195.f}, color, "Shoot: 'K'", NULL);
        ImDrawList_AddText_Vec2(drawList, (ImVec2){595.f, 210.f}, color, "Teleport: 'U' or 'O'", NULL);
    }

    ImDrawList_AddText_Vec2(drawList, (ImVec2){300.f, 400.f}, color, "Do you want to play ?", NULL);
    {
        color = igGetColorU32_Vec4(((ImVec4){0.f, 0.3f, 1.f, 1.f}));
        ImDrawList_AddText_Vec2(drawList, (ImVec2){300.f, 415.f}, color, "Press 'F'   : Solo mode", NULL);
        color = igGetColorU32_Vec4(((ImVec4){0.f, 1.f, 0.f, 1.f}));
        ImDrawList_AddText_Vec2(drawList, (ImVec2){300.f, 430.f}, color, "Press 'K'   : Two player mode", NULL);
        color = igGetColorU32_Vec4(((ImVec4){1.f, 0.f, 0.f, 1.f}));
        ImDrawList_AddText_Vec2(drawList, (ImVec2){300.f, 445.f}, color, "Press 'Esc' : Exit", NULL);
    }
}

void draw_game_menu(/*bool 2player?, int lives_p1, int lives_p2, int score*/)
{
    color = igGetColorU32_Vec4(((ImVec4){0.f, 0.8f, 1.0f, 1.0f}));
    ImDrawList_AddRectFilled(drawList, (ImVec2){20.f, 120.f}, (ImVec2){160.f, 160.f}, color, 10.f, ImDrawFlags_RoundCornersAll);
    // draw lives p1

    // ADD if game 2player
    color = igGetColorU32_Vec4(((ImVec4){0.f, 1.0f, 0.1f, 1.0f}));
    ImDrawList_AddRectFilled(drawList, (ImVec2){590.f, 120.f}, (ImVec2){730.f, 160.f}, color, 10.f, ImDrawFlags_RoundCornersAll);
    // draw lives p2

    // score

    //space for pause ?
}

void draw_pause_menu()
{
}

void draw_gameover_menu()
{
}

void draw_menu(Menu menu)
{
    drawList = igGetForegroundDrawList_Nil();
    switch (menu)
    {
    case PLAY:
        draw_play_menu();
        break;

    case GAME:
        draw_game_menu();
        break;

    case PAUSE:
        draw_pause_menu();
        break;

    case GAMEOVER:
        draw_gameover_menu();
        break;

    default:
        break;
    }
}