/* Program file initializes ncurses screen */

#include "log.h"
#include "stdheader.h"

/* Function initializes ncurses screen */
void tui_init() {

  setlocale(LC_ALL, "");
  initscr();
  noecho();
  raw();
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL);
  nodelay(stdscr, true);
}
