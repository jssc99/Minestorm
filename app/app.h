#include "player.h"
#include "enemy.h"

#define TOLERANCE 0.00001

typedef struct App
{
    Player player1;
} App;

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
