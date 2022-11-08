#include <stdbool.h>
#include <math.h>
#include "player.h"

#define TOLERANCE 0.00001

typedef struct App
{
} App;

void appInit(App *app);
void appUpdate(App *app);
void appShutdown(App *app);
