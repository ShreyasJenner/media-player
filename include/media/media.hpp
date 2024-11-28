#ifndef MEDIA_H
#define MEDIA_H

namespace MediaError {
enum MediaError { NOERROR, WARN, ERROR };
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

#endif /* ifndef MEDIA _H                                                      \
#define MEDIA_H */
