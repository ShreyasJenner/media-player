#ifndef MEDIA_H
#define MEDIA_H

#include "media/header.hpp"

namespace MediaError {
enum MediaError { NOERROR, WARN, ERROR };
}

class Media {
private:
  MediaError::MediaError error;
  Mix_Music *music;

public:
  Media();

  void init();

  void openAudio(char *track);

  void play(char *track);

  void pause();

  void resume();

  void rewind();

  void setPosition(double pos);

  void freeMusic();

  void deinit();

  int getError();
};

#endif /* ifndef MEDIA _H                                                      \
#define MEDIA_H */
