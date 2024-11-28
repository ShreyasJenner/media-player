#ifndef PLAY_H
#define PLAY_H

class Media {
public:
  Media();

  void init();

  void play(char *track);

  void pause();

  void resume();

  void deinit();
};

#endif /* ifndef PLAY_H                                                        \
#define PLAY_H */
