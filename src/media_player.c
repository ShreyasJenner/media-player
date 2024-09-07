#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <locale.h>
#include <stdio.h>
#include <ncurses.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/* header files */
#include "structs.h"
#include "mp3_lut.h"
#include "key.h"

/* external header files */
#include "id3reader/include/id3reader.h"
#include "progress-bar/include/progress_bar.h"


/*
 * 
 */
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
        } else if(ch == KEY_RESIZE) {
            reprint_pb((int)Mix_GetMusicPosition(music), (int)Mix_MusicDuration(music), 0);
        }

        mvprintw(0, 0, "%d\n",(int)Mix_GetMusicPosition(music));

        progress_bar((int)Mix_GetMusicPosition(music), (int)Mix_MusicDuration(music),0);

        SDL_Delay(100);
    }
}

int main(int argc, char **argv)  {
    int result, flags, fd;
    uint8_t frame_header[4];
    struct mp3_frame_header_data mfhd;

    /* external struct for storing id3 tag */
    struct id3_tag *tag;

    /* read id3 tag from file */
    fd = open(argv[1], O_RDONLY, 0);
    if(fd<0) {
        printf("Opening file erorr\n");
        exit(1);
    }
    tag = get_id3tag(fd);
    if(tag == NULL) {
        printf("id3 tag not available\n");
    } else {
        /* CURRENT CODE only supports getting mp3 frames through
         * id3 tags */
        /* read frame header bytes of 1st mp3 frame */
        lseek(fd, tag->size, SEEK_SET);
        read(fd, frame_header, 4);
        if(get_mp3FrameHeader(frame_header, &mfhd)<0) {
            printf("File isnt mp3\n");
        }
    }

        /* initalize sdl mixer */
    result = 0;
    flags = MIX_INIT_MP3|MIX_INIT_FLAC;

    if(SDL_Init(SDL_INIT_AUDIO)<0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    if(flags != (result = Mix_Init(flags))) {
        printf("Could not initalize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    /* use mp3 frame header data to set default settings */
    Mix_OpenAudioDevice(mfhd.samplerate, AUDIO_S16SYS, mfhd.channel_no, 4096, NULL, 0);

    /* load the music file */
    Mix_Music *music = Mix_LoadMUS(argv[1]);
    if(music==NULL) {
        printf("Error opening music file\n");
        exit(1);
    }

    /* set up ncurses settings */
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
    Mix_CloseAudio();
    SDL_Quit();
    endwin();

    return 0;
}
