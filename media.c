#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_quit.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <pthread.h>

#include "progress-bar/struct.h"
#include "progress-bar/writer.h"
#include "progress-bar/progress_bar.h"

void* play_music(Mix_Music *music) {
    // get the length of the music file in seconds
    printf("Length of file:%lf\n",Mix_MusicDuration(music));

    // call the init function for writer
    struct dimensions *dm = init((int)Mix_MusicDuration(music));


    // play the audio file
    Mix_PlayMusic(music, 0);

    while(!SDL_QuitRequested()) {
        //printf("%1.0f", Mix_GetMusicPosition(music));
        get_data(dm, (int)Mix_GetMusicPosition(music));
        SDL_Delay(1000);
        //fflush(stdout);
        //printf("\r");
       
        // break out of loop if music is playing
        if(Mix_PlayingMusic()==0)
            break;
    }

    // detach the shared memory
    finish(dm);

    return 0;
}

int main()  {
    int result, flags;
    double len;
    pthread_t thread_id;

    result = 0;
    flags = MIX_INIT_MP3;

    if(SDL_Init(SDL_INIT_AUDIO)<0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    if(flags != (result = Mix_Init(flags))) {
        printf("Could not initalize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    // Open audio stream
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    // load the music file
    Mix_Music *music = Mix_LoadMUS("muisc-files/sample.mp3");

    // run the progress bar through another thread
    pthread_create(&thread_id, NULL, progress_bar, NULL);

    // play the music file
    play_music(music);

    // free the music object
    Mix_FreeMusic(music);

    // clear up all initaliazed subsystems
    SDL_Quit();

    pthread_join(thread_id, NULL);

    return 0;
}
