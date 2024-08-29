#include <ncurses.h>
#include <SDL2/SDL_mixer.h>

#include "../include/key.h"

void* key_detect(void *music) {
    int ch, flag;

    ch = '\0';
    flag = 1;

    while(ch!='q') {
        ch = getch();

        if(ch==32 && flag) {
            Mix_PauseMusic();
            flag = 0;
        }
        else {
            Mix_ResumeMusic(); 
            flag = 1;
        }
    }

    return 0;
}
