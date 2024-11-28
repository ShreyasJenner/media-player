#include "media/play.hpp"
#include "media/media.hpp"

/* Default constructor for media calls init */
Media::Media() {
  // initialize sdl_mixer and audio devices
  init();
}

/* Function to sdl mixer and initialize audio devices */
void Media::init() {
  // allow playing of flac and mp3 files
  int flags = MIX_INIT_FLAC | MIX_INIT_MP3;
  // TODO: add error checking and log error
  Mix_Init(flags);

  // TODO: get app metadata to set frequency and format
  // NOTE: experiment with chunk size to get best result
  // TODO: add error checking and log errors
  Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048);
}

/* Function to play music file */
void Media::play(char *track) {
  Mix_Music *music;

  // load track
  // TODO: add error checking and log erors
  music = Mix_LoadMUS(track);

  // play track
  // TODO: add error checking and log erors
  Mix_PlayMusic(music, 0);
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
