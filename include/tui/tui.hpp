#ifndef TUI_HPP
#define TUI_HPP

#include "FileSystem/FileSystem.hpp"
#include "tui/section.hpp"

extern "C" {
#include <sys/ioctl.h>
#include <unistd.h>
}

class TUI {
private:
  FileSystem *fs;
  Section1 section1;
  Section2 section2;
  int term_rows, term_cols;

public:
  TUI(FileSystem *fs);

  void get_term_size();

  void create_sec1();

  void create_sec2();

  void run();

  ~TUI();
};

#endif // !TUI_HPP
