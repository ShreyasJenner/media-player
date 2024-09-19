/* Program file handles all things related to using the sdl library
 * such as initalization, deinitialization, playing music file, etc... */

#include "media-handling/sdl.h"
#include "log.h"
#include "progress-bar/include/progress_bar.h"
#include "stdheader.h"
#include <SDL2/SDL_mixer.h>
#include <curses.h>

/* Function initialized sdl with options to play
 * certain music files
 * Returns 0 on success */
int sdl_init() {
  /* Declaration */
  int flags;

  /* Initalization */
  flags = MIX_INIT_MP3 | MIX_INIT_FLAC;

  /* Initalize sdl audio */
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    logerror(__func__, "Failed to initalize SDL");
    return -1;
  }

  /* Check if flags have been initalized correctly */
  if (flags != Mix_Init(flags)) {
    logerror(__func__, "Could not initalize mixer");
    logerror(__func__, (char *)Mix_GetError());
    return -1;
  }

  // TODO: Use metadata file reader to set accurate parameters for audio device
  //
  /* Set default settings for audio device */
  if (Mix_OpenAudioDevice(48000, AUDIO_S16SYS, 2, 4096, NULL, 0) != 0) {
    logerror(__func__, "Error opening audio device");
    return -1;
  }

  return 0;
}

/* Function to load and start music file */
Mix_Music *load_music(char *selfile) {
  /* Declaration */
  Mix_Music *music;

  /* Load music file */
  music = Mix_LoadMUS(selfile);

  if (music == NULL) {
    logerror(__func__, "Error opening music file");
    return NULL;
  }

  /* Start playing the music */
  Mix_PlayMusic(music, 0);

  return music;
}

/* Function to handle key presses for sdl playing
 * Returns the value of flag */
int sdl_HandleKeypress(Mix_Music *music, int key, int flag) {

  switch (key) {
  case 'q':
    return MEDIA_PLAYER_QUIT;
    break;
  case SPACEBAR_PRESS:
    // NOTE: flag = 1 => audio playing
    //  flag = 0 => audio paused
    if (flag) {
      Mix_PauseMusic();
      flag = 0;
    } else {
      Mix_ResumeMusic();
      flag = 1;
    }
    break;

  case KEY_RIGHT:
    /* Ensure music position can be set to music duration at maximum */
    Mix_SetMusicPosition(
        (Mix_GetMusicPosition(music) + 5 >= Mix_MusicDuration(music)
             ? Mix_MusicDuration(music)
             : Mix_GetMusicPosition(music) + 5));
    break;

  case KEY_LEFT:
    /* Ensure music position can be set to 0 at min */
    Mix_SetMusicPosition((Mix_GetMusicPosition(music) - 5 >= 0
                              ? Mix_GetMusicPosition(music) - 5
                              : 0));
    break;

  case KEY_RESIZE:
    reprint_pb((int)Mix_GetMusicPosition(music), (int)Mix_MusicDuration(music),
               0);
    break;

  default:
    break;
  }

  return flag;
}

/* Function to close music files, audio devices and deinitialize sdl */
void sdl_deinit(Mix_Music *music) {
  Mix_FreeMusic(music);
  Mix_CloseAudio();
  SDL_Quit();
}
