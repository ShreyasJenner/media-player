#include <iostream>

#include "log/log.hpp"
#include "media/media.hpp"
#include "media/play.hpp"

/* Default constructor for media calls init */
Media::Media() {
  // initialize sdl_mixer and audio devices
  init();
}

/* Function to sdl mixer and initialize audio devices */
void Media::init() {
  // allow playing of flac and mp3 files
  int flags = MIX_INIT_FLAC | MIX_INIT_MP3;

  // error checking
  if (Mix_Init(flags) != flags) {
    this->error = MediaError::ERROR;
    logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
             "SDL Mixer Initialization");
    return;
  }

  // error checking
  // TODO: get app metadata to set frequency and format
  // NOTE: experiment with chunk size to get best result
  if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048) != 0) {
    this->error = MediaError::ERROR;
    logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
             "SDL Opening audio device failed");
    return;
  }
}

/* Function to play music file */
void Media::play(char *track) {
  Mix_Music *music;

  // load track
  music = Mix_LoadMUS(track);

  // error checking
  if (music == NULL) {
    this->error = MediaError::ERROR;
    logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
             "Loading track failed");
    return;
  }

  // play track
  if (Mix_PlayMusic(music, 0) != 0) {
    this->error = MediaError::ERROR;
    logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
             "Playing track failed");
    return;
  }
}

/* Function to pause track */
void Media::pause() {
  // pause the track
  Mix_PauseMusic();
}

/* Function to resume the track */
void Media::resume() {
  // resume the track
  Mix_ResumeMusic();
}

/* Function to deinitalize audio devices and close sdl mixer */
void Media::deinit() {
  // close audio device
  Mix_CloseAudio();

  // deinitalize sdl mixer
  Mix_Quit();
}

/* Function to get error value from object */
int Media::getError() { return this->error; }
