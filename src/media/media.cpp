#include <iostream>

#include "log/log.hpp"
#include "media/header.hpp"
#include "media/media.hpp"

extern "C" {
#include "audio-metadata-reader/flac/read_flac_metadata.h"
#include "audio-metadata-reader/mp3/id3_structs.h"
#include "audio-metadata-reader/mp3/id3reader.h"
#include "audio-metadata-reader/mp3/mp3_lut.h"
#include "audio-metadata-reader/stdheader.h"

#include <stdio.h>
}

/* Default constructor for media calls init */
Media::Media() {
  // initialize sdl_mixer and audio devices
  init();
}

/* Function to start sdl mixer */
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
}

/* Function to initalize audio device with the sample rates and other values
 * obtained from the media file */
void Media::openAudio(char *track) {
  MP3FrameHeader *mfhd;
  FLACMetadata *metadata;
  int chunksize;
  uint16_t format;

  // TODO: experiment with chunksize to get best results
  chunksize = 2048;

  // set audio format to safe default
  format = AUDIO_S16SYS;

  // TODO: make code format agnostic

  // check if file is mp3 file
  if (id3_tagcheck(track)) {
    // get mp3 frame header
    mfhd = get_mp3FrameHeader(track);
    if (mfhd == NULL) {
      // set warning if failed to get the mp3 frame header data from the file
      // and set audio device to sane defaults
      this->error = MediaError::WARN;
      logerror(__FILE__, __LINE__, __func__, LOGLEVEL::WARN,
               "Error getting mp3 frame header data");

    } else {

      // use the mp3 format data to set the audio device values
      if (Mix_OpenAudio(mfhd->samplerate, format, mfhd->channel_no,
                        chunksize) != 0) {
        this->error = MediaError::ERROR;
        logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
                 "SDL Opening audio device failed");
        return;
      }

      // clean up mp3 frame header data
      MP3FrameHeader_FREE(mfhd);
    }
    // check if file is  flac file
  } else if (flac_check(track)) {

    // get flac metadata
    metadata = get_FLACMetadata(track);
    if (metadata == NULL) {
      // set warning if failed to get flac metadata and set audio device to sane
      // defaults
      this->error = MediaError::WARN;
      logerror(__FILE__, __LINE__, __func__, LOGLEVEL::WARN,
               "Error getting flac metadata");
    } else {

      // open audio device using flac metadata
      if (Mix_OpenAudio(metadata->streaminfo->data.stream_info.sample_rate,
                        format, metadata->streaminfo->data.stream_info.channels,
                        chunksize) != 0) {
        this->error = MediaError::ERROR;
        logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
                 "SDL Opening audio device failed");
        return;
      }

      // BUG: calling clean flac metadata causes free error
      //  clean flac metadata
      //  clean_FLACMetadata(metadata);
    }

    // set the audio device to default parameters if warning is present in media
    if (this->getError() == MediaError::WARN) {
      this->error = MediaError::NOERROR;
      if (Mix_OpenAudio(48000, format, 2, chunksize) != 0) {
        this->error = MediaError::ERROR;
        logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
                 "SDL Opening audio device failed");
        return;
      }
    }
  }
}

/* Function to play the music file */
void Media::play(char *track) {
  Mix_Music *music;

  // load track
  this->music = Mix_LoadMUS(track);

  // error checking
  if (this->music == NULL) {
    this->error = MediaError::ERROR;
    logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
             "Loading track failed");
    return;
  }

  // play track
  if (Mix_PlayMusic(this->music, 0) != 0) {
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

/* Function to rewind track to start */
void Media::rewind() {
  // rewind music to start of track
  Mix_RewindMusic();
}

/* Function to set position in track */
void Media::setPosition(double pos) {
  // set position of track
  if (Mix_SetMusicPosition(pos) != 0) {
    this->error = MediaError::WARN;
    logerror(__FILE__, __LINE__, __func__, LOGLEVEL::WARN,
             "Failed to set track position");
    return;
  }
}

/* Function to free the music object */
void Media::freeMusic() {
  // free the music object
  Mix_FreeMusic(this->music);
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
