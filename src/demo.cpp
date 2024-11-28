#include <stdio.h>
#include <unistd.h>

#include <thread>

#include "log/log.hpp"
#include "media/media.hpp"
#include "media/play.hpp"

int main(int argc, char **argv) {
  // check if argument has been passed
  if (argc < 2) {
    printf("Arguments not passed\n");
    return 1;
  }

  // create media object
  /*Media med;*/
  /**/
  /*std::thread t1(&Media::play, &med, argv[1]);*/
  /*// med.play(argv[1]);*/
  /*sleep(5);*/
  /*med.pause();*/
  /*printf("Pausing\n");*/
  /*sleep(5);*/
  /*med.resume();*/
  /*printf("Resuming\n");*/
  /*sleep(5);*/
  /*med.deinit();*/
  /*printf("Ending\n");*/
  /**/
  /*// wait for thread t1 to finish*/
  /*t1.join();*/
  /**/
  /*// testing logging*/
  /*logerror(__FILE__, __LINE__, __func__, INFO, "Demo error message");*/
}
