#include <filesystem>
#include <thread>
#include <vector>

#include "FileSystem/FileSystem.hpp"
#include "FileSystem/TrieDS.hpp"
#include "key_event/key_event.hpp"
#include "log/log.hpp"
#include "media/header.hpp"
#include "media/media.hpp"

extern "C" {
#include <stdio.h>
#include <unistd.h>
}

int main(int argc, char **argv) {
  // check if argument has been passed
  /*if (argc < 2) {*/
  /*  printf("Arguments not passed\n");*/
  /*  return 1;*/
  /*}*/

  // NOTE:media module demo
  /*Media med;*/
  /**/
  /*if (med.getError() == MediaError::ERROR) {*/
  /*  return 1;*/
  /*}*/
  /**/
  /*med.openAudio(argv[1]);*/
  /*if (med.getError() == MediaError::ERROR) {*/
  /*  return 1;*/
  /*}*/
  /**/
  /*std::thread t1(&Media::play, &med, argv[1]);*/
  /**/
  /*// wait for 1 second for the thread to start*/
  /*sleep(1);*/
  /**/
  /*while (Mix_PlayingMusic()) {*/
  /*}*/
  /**/
  /*med.freeMusic();*/
  /*med.deinit();*/
  /**/
  /*t1.join();*/

  // NOTE: logging module demo
  /*logerror(__FILE__, __LINE__, __func__, LOGLEVEL::INFO, "Demo error
   * message");*/

  // NOTE: file system
  FileSystem fs("/media");

  fs.createMediaTree();
  fs.createTrie();

  /*std::vector<std::string> temp = fs.getTrie()->searchWord("Mas");*/
  /*for (std::string word : temp) {*/
  /*  std::cout << word << "\n";*/
  /*}*/

  fs.getMediaTree()->displayMediaTree();

  return 0;
}
