#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_quit.h>
#include <SDL2/SDL_timer.h>
#include <locale.h>
#include <stdio.h>
#include <pthread.h>
#include <ncurses.h>

#include "../include/key.h"
#include "../external/progress-bar/include/progress_bar.h"

void play_music(Mix_Music *music) {
    int ch;
    int flag;

    flag = 1;
    // play the audio file
    Mix_PlayMusic(music, 0);

    while(!SDL_QuitRequested()) {
        // music is over
        if(!Mix_PlayingMusic())
            break;

        ch = getch();
        if(ch == 'q')
            break;
        else if(ch == 32) {
            if(flag == 1) {
                Mix_PauseMusic();
                flag = 0;
            } else {
                Mix_ResumeMusic();
                flag = 1;
            }
            ch = '\0';
        } else if(ch == 261) {
            Mix_SetMusicPosition((Mix_GetMusicPosition(music)+5));
        } else if(ch == 260) {
            Mix_SetMusicPosition((Mix_GetMusicPosition(music)-5));
        }

        mvprintw(0, 0, "%d\n",(int)Mix_GetMusicPosition(music));
        progress_bar(Mix_GetMusicPosition(music), Mix_MusicDuration(music));

        SDL_Delay(10);
    }
}

int main(int argc, char **argv)  {
    int result, flags, *quit;
    double len;
    pthread_t thread_id;

    result = 0;
    flags = MIX_INIT_MP3|MIX_INIT_FLAC;


    // Initalize SDL with audio sub-system
    if(SDL_Init(SDL_INIT_AUDIO)<0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    // Check if sdl mixer has managed to load mp3 library needed
    if(flags != (result = Mix_Init(flags))) {
        printf("Could not initalize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    // default settings
    Mix_OpenAudio(41000, AUDIO_S16SYS, 2, 1024);
    // load the music file
    Mix_Music *music = Mix_LoadMUS(argv[1]);

    if(music==NULL) {
        printf("Error opening music file\n");
        exit(1);
    }

    // set up ncurses settings
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    nodelay(stdscr, true);

    Mix_PlayMusic(music, 0);

    // create a thread to detect key presses
    //thread_id = pthread_create(&thread_id, NULL, key_detect, &music); 

    play_music(music);

    // block program until quit is pressed
    //pthread_join(thread_id, NULL);

    Mix_FreeMusic(music);
    SDL_Quit();
    endwin();

    return 0;
}
