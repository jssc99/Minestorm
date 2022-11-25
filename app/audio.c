#include "audio.h"
#include "app.h"

#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio.h"
/*
void music_stop(Music* music)
{
    ma_sound_stop(&music->current);
}

void music_play(Music *music)
{
    ma_sound current = music->current;
    ma_sound_start(&current);
}

void music_change(ma_sound* toPlay, Music *music)
{
    ma_sound_stop(&music->current);
    music->current = *toPlay;
    ma_sound_start(&music->current);
    
}
*/