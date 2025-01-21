#include <chrono>
#include <filesystem>
#include <locale>
#include <thread>
#include <vector>

#include "FileSystem/FileSystem.hpp"
#include "FileSystem/TrieDS.hpp"

#include "tui/tui.hpp"

#include "log/log.hpp"
#include "media/header.hpp"
#include "media/media.hpp"

extern "C" {
#include <stdio.h>
#include <unistd.h>
}

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");

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
  FileSystem fs(L"/media", FORMATTED_DIR_STRUCTURE);

  // std::vector<Node *> temp = fs.getMediaTree()->getAlbumPtrs();
  // for (auto tem : temp) {
  //   std::wcout << tem->wpath << '\n';
  // }
  // std::wcout << temp[55]->wpath << '\n';

  // NOTE: tui system
  TUI tui(&fs);
  tui.run();

  return 0;
}
