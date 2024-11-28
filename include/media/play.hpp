#ifndef PLAY_H
#define PLAY_H

namespace MediaError {
enum MediaError { WARN, ERROR };
}

class Media {
private:
  MediaError::MediaError error;

public:
  Media();

  void init();

  void play(char *track);

  void pause();

  void resume();

  void deinit();

  int getError();
};

#endif /* ifndef PLAY_H                                                        \
#define PLAY_H */
