#ifndef SDL_H
#define SDL_H

#include "stdheader.h"

#define MEDIA_PLAYER_QUIT -2
#define SPACEBAR_PRESS 32

int sdl_init();

Mix_Music *load_music(char *selfile);

int sdl_HandleKeypress(Mix_Music *music, int key, int flag);

void sdl_deinit(Mix_Music *music);

#endif // !SDL_H
