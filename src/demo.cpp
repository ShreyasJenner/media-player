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

  // NOTE:media module demo
  Media med;

  if (med.getError() == MediaError::ERROR) {
    return 1;
  }

  std::thread t1(&Media::play, &med, argv[1]);

  sleep(2);
  if (med.getError() == MediaError::ERROR) {
    t1.join();
    med.deinit();
    return 1;
  }

  sleep(5);
  med.pause();
  printf("Pausing\n");
  sleep(5);
  med.resume();
  printf("Resuming\n");
  sleep(5);
  med.deinit();
  printf("Ending\n");

  t1.join();

  // NOTE: logging module demo
  /*logerror(__FILE__, __LINE__, __func__, LOGLEVEL::INFO, "Demo error
   * message");*/
}
