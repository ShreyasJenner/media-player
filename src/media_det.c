#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_quit.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <pthread.h>

int main() {
    int result, flags;
    double len;
    
    result = 0;
    flags = MIX_INIT_MP3;

    if(SDL_Init(SDL_INIT_AUDIO)<0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    result = Mix_Init(flags);
    if(flags != result) {
        printf("Could not initalize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    // Open audio stream
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    // load the music file
    Mix_Music *music = Mix_LoadMUS("music-files/sample2.mp3");

    printf("Music Title:%s\n",Mix_GetMusicTitle(music));
    printf("Music Artist Tag:%s\n", Mix_GetMusicArtistTag(music));
    printf("Music Album Tag:%s\n", Mix_GetMusicAlbumTag(music));

    // free the music object
    Mix_FreeMusic(music);

    // clear up all initaliazed subsystems
    SDL_Quit();

    return 0;
}
