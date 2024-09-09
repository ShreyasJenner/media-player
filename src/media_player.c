/* header files */
#include "stdheader.h"
#include "structs.h"
#include "tree.h"
#include "log.h"
#include "mp3_lut.h"

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
    /* Declaration */
    int result, flags;

    /* temp code */
    struct Tree tree;
    int matches, choice;
    result = get_DirTree(&tree, argv[1]);
    if(result!=0) {
        logerror(__func__, "Calling get_DirTree");
        return 0;
    }
    char pattern[FILE_NAME_SZ];
    char filelist[tree.filecount][FILE_NAME_SZ];
    printf("Tree dirs:%d\n",tree.dircount);
    printf("Tree files:%d\n",tree.filecount);

    scanf("%[^\n]",pattern);
    matches = search_Tree(tree, pattern, filelist);
    
    if(matches==0 || !strcmp(pattern,"-1")) {
        printf("No matches\n");
        free_Tree(&tree.root);
        exit(1);
    }
    for(int i=0;i<matches;i++)
        printf("%d:%s\n",i,filelist[i]);
    scanf("%d",&choice);
    printf("Chosen file:%s\n",filelist[choice]);

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
    Mix_OpenAudioDevice(48000, AUDIO_S16SYS, 2, 4096, NULL, 0);

    /* load the music file */
    Mix_Music *music = Mix_LoadMUS(filelist[choice]);
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

    /* play music with 0 loops */
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

    /* temp code */
    free_Tree(&tree.root);
    /* temp code */

    return 0;
}
