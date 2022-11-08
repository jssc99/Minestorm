#include <stdbool.h>
#include <math.h>
#include "Tools/matrix.h"
#include "player.c"

#define TOLERANCE 0.00001

typedef struct App
{
} App;

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
