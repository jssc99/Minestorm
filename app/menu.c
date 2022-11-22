#include "menu.h"

#define WIDTH 700.f
#define WIDTH_2 350.f
#define HEIGHT 800.f
#define HEIGHT_2 400.f

#define DRAW_LIST igGetForegroundDrawList_Nil()

#define GREY igGetColorU32_Vec4((ImVec4){1.0f, 1.0f, 1.0f, 0.5})
#define WHITE igGetColorU32_Vec4((ImVec4){1.0f, 1.0f, 1.0f, 1.0f})
#define RED igGetColorU32_Vec4((ImVec4){1.0f, 0.0f, 0.0f, 1.0f})
#define DEEP_BLUE igGetColorU32_Vec4((ImVec4){0.0f, 0.0f, 1.0f, 1.0f})
#define BLUE igGetColorU32_Vec4((ImVec4){0.2f, 0.2f, 1.0f, 1.0f})
#define GREEN igGetColorU32_Vec4((ImVec4){0.0f, 1.0f, 0.0f, 1.0f})
#define PLAYER1 igGetColorU32_Vec4((ImVec4){0.0f, 0.8f, 1.0f, 1.0f})
#define PLAYER2 igGetColorU32_Vec4((ImVec4){0.0f, 1.0f, 0.5f, 1.0f})

// debug for alignment
void x_middle_line(void)
{
    ImDrawList_AddLine(DRAW_LIST, (ImVec2){WIDTH_2, 0}, (ImVec2){WIDTH_2, HEIGHT}, WHITE, 2.f);
}
void y_middle_line(void)
{
    ImDrawList_AddLine(DRAW_LIST, (ImVec2){0, HEIGHT_2}, (ImVec2){WIDTH, HEIGHT_2}, WHITE, 2.f);
}
void middle_lines(void)
{
    x_middle_line();
    y_middle_line();
}

void draw_player_box(bool p2)
{
    ImDrawList_AddRectFilled(DRAW_LIST, (ImVec2){20.f, 120.f}, (ImVec2){160.f, 160.f}, PLAYER1, 10.f, ImDrawFlags_RoundCornersAll);
    if (p2)
        ImDrawList_AddRectFilled(DRAW_LIST, (ImVec2){540.f, 120.f}, (ImVec2){680.f, 160.f}, PLAYER2, 10.f, ImDrawFlags_RoundCornersAll);
}

void draw_instruction(ImFont *font, bool p2)
{
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 30.f, (ImVec2){30.f, 125.f}, WHITE, "Player One", NULL, 0.f, NULL);
    ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){25.f, 165.f}, WHITE, "Rotate: 'D' & 'G'", NULL);
    ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){25.f, 180.f}, WHITE, "Move: 'R'", NULL);
    ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){25.f, 195.f}, WHITE, "Shoot: 'F'", NULL);
    ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){25.f, 210.f}, WHITE, "Teleport: 'E' or 'T'", NULL);

    if (p2)
    {
        ImDrawList_AddText_FontPtr(DRAW_LIST, font, 30.f, (ImVec2){548.f, 125.f}, WHITE, "Player Two", NULL, 0.f, NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){545.f, 165.f}, WHITE, "Rotate: 'J' & 'L'", NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){545.f, 180.f}, WHITE, "Move: 'I'", NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){545.f, 195.f}, WHITE, "Shoot: 'K'", NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){545.f, 210.f}, WHITE, "Teleport: 'U' or 'O'", NULL);
    }
}

void draw_score(ImFont *font, int score, float x, float y, ImU32 color)
{
    char textTemp[30];
    sprintf(textTemp, "Score : %8d", score);
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 35.f, (ImVec2){x, y}, color, textTemp, NULL, 0.f, NULL);
}

void draw_play_menu(ImFont *font)
{
    draw_player_box(1);
    draw_instruction(font, 1);
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 35.f, (ImVec2){220.f, 360.f}, WHITE, "Do you want to play ?", NULL, 0.f, NULL);
    {
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){255.f, 410.f}, BLUE,  "Press 'F'    :   Solo mode", NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){255.f, 425.f}, GREEN, "Press 'K'    :   Two player mode", NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){255.f, 440.f}, RED,   "Press 'Esc'  :   Exit Game", NULL);
    }
}

void draw_lives(ImFont *font, int lives, float x, float y)
{
    char textTemp[30];
    sprintf(textTemp, "Lives : %d", lives);
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 30.f, (ImVec2){x, y}, WHITE, textTemp, NULL, 0.f, NULL);
}

void draw_in_game_menu(ImFont *font, bool p2, int lives_p1, int lives_p2, int score, int level)
{
    draw_player_box(p2);
    draw_lives(font, lives_p1, 40.f, 125.f);
    if (p2)
        draw_lives(font, lives_p2, 565.f, 125.f);
    draw_score(font, score, 250.f, 760.f, WHITE);
    char textTemp[30];
    sprintf(textTemp, "Level : %d", level);
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 20.f, (ImVec2){10.f, 780.f}, WHITE, textTemp, NULL, 0.f, NULL);
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 20.f, (ImVec2){580.f, 780.f}, WHITE, "'Space' : PAUSE", NULL, 0.f, NULL);
}

void draw_pause_menu(ImFont *font, int score, bool p2)
{
    draw_player_box(p2);
    draw_instruction(font, p2);
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 35.f, (ImVec2){302.f, 360.f}, WHITE, "PAUSE", NULL, 0.f, NULL);
    {
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){250.f, 410.f}, BLUE, "Press 'SPACE' :   Resume Game", NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){250.f, 425.f}, RED,  "Press 'Esc'   :   Main Menu", NULL);
    }
    draw_score(font, score, 250.f, 760.f, WHITE);
}

void draw_gameover_menu(ImFont *font, int score)
{
    draw_player_box(1);
    draw_instruction(font, 1);
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 40.f, (ImVec2){252.f, 360.f}, WHITE, "GAME OVER", NULL, 0.f, NULL);
    {
        draw_score(font, score, 265.f, 400.f, GREEN);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){250.f, 450.f}, BLUE, "Press 'SPACE' :   New Game", NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){250.f, 465.f}, RED,  "Press 'Esc'   :   Exit Game", NULL);
    }
}

void draw_success_menu(ImFont *font, int score, bool p2, int lives_p1, int lives_p2)
{
    draw_player_box(p2);
    draw_lives(font, lives_p1, 45.f, 125.f);
    if (p2)
        draw_lives(font, lives_p2, 565.f, 125.f);
    ImDrawList_AddText_FontPtr(DRAW_LIST, font, 40.f, (ImVec2){218.f, 360.f}, WHITE, "GREAT SUCCESS", NULL, 0.f, NULL);
    {
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){250.f, 410.f}, BLUE, "Press 'SPACE' :   Continue Game", NULL);
        ImDrawList_AddText_Vec2(DRAW_LIST, (ImVec2){250.f, 425.f}, RED,  "Press 'Esc'   :   Main Menu", NULL);
    }
    draw_score(font, score, 250.f, 760.f, WHITE);
}

// draws any menu inputed, respect the input demanded
void draw_menu(Menu menu, ImFont *font, int score, int level, bool p2, int p1_lives, int p2_lives)
{
    //middle_lines();
    switch (menu)
    {
    case MAIN:
        draw_play_menu(font);
        break;

    case IN_GAME:
        draw_in_game_menu(font, p2, p1_lives, p2_lives, score, level);
        break;

    case PAUSE:
        draw_pause_menu(font, score, p2);
        break;

    case GAMEOVER:
        draw_gameover_menu(font, score);
        break;

    case SUCCESS:
        draw_success_menu(font, score, p2, p1_lives, p2_lives);
        break;

    default:
        break;
    }
}