#include "tui/tui.hpp"

/* Default Constructor for TUI */
TUI::TUI() {

  // initialize curses settings
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  refresh();

  // store the terminal dimensions
  this->get_term_size();
}

/* Function to get terminal dimensions */
void TUI::get_term_size() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  this->term_rows = w.ws_row;
  this->term_cols = w.ws_col;
}

/* Function to create section1 */
void TUI::create_sec1() {
  this->section1.init(this->term_rows, this->term_cols / 3, 0, 0);
  this->section1.create_menu();
  touchwin(this->section1.get_win());
  wrefresh(this->section1.get_win());
}

/* Function to run the tui */
void TUI::run() {

  // create the sections
  this->create_sec1();

  // NOTE: temp code to refresh screen so that we can see curses output
  update_panels();
  doupdate();

  // NOTE: temp code that allows moving menu selection
  int ch;
  ch = 'a';
  while (ch != 'q') {
    ch = getch();
    this->section1.menu_driver(ch);
  }
}

/* Destructor for TUI class */
TUI::~TUI() { endwin(); }
