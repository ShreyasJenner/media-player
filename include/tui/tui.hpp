#ifndef TUI_HPP
#define TUI_HPP

#include "tui/section.hpp"

extern "C" {
#include <sys/ioctl.h>
#include <unistd.h>
}

class TUI {
private:
  Section1 section1;
  int term_rows, term_cols;

public:
  TUI();

  void get_term_size();

  void create_sec1();

  void run();

  ~TUI();
};

#endif // !TUI_HPP
