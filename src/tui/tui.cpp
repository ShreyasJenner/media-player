#include "tui/tui.hpp"

/* Default Constructor for TUI */
TUI::TUI(FileSystem *fs) {

  // initialize curses settings
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  refresh();

  // store the FileSystem pointer and terminal dimensions
  this->fs = fs;
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

  // initialize and create windows
  this->section1.init(1);
  this->section1.init(this->term_rows, this->term_cols / 3, 0, 0);

  // create the option menu
  this->section1.create_menu();

  // render window onto screen
  touchwin(this->section1.get_win()[0]);
  wrefresh(this->section1.get_win()[0]);
}

/* Function to create section2 */
void TUI::create_sec2() {
  int i;

  // initialize and create windows
  this->section2.init(SECTION_2_WIN_COUNT);
  this->section2.init(this->term_rows, (2 * this->term_cols) / 3,
                      this->term_cols / 3, 0);

  // render windows onto screen
  for (i = 0; i < SECTION_2_WIN_COUNT; i++) {
    touchwin(this->section2.get_win()[i]);
    wrefresh(this->section2.get_win()[i]);
  }

  // create different windows for each option in Section1
  this->section2.create_sec1_artist_window(
      this->fs->getMediaTree()->getArtistPtrs());
}

/* Function to run the tui */
void TUI::run() {

  // create the sections
  this->create_sec1();
  this->create_sec2();

  // NOTE: temp code to refresh screen so that we can see curses output
  update_panels();
  doupdate();

  // NOTE: temp code that allows moving menu selection
  int ch;
  ch = 'a';
  while (ch != 'q') {
    ch = getch();
    this->section1.menu_driver(ch);

    // NOTE: temp code to handle enter key press
    if (ch == KEY_ENTER || ch == 10) {
      this->section2.render_selected_win(
          item_name(this->section1.get_selected_item()));

      // NOTE: temp code to run menu driver for artist window
      if (!strcmp("Artist", item_name(this->section1.get_selected_item()))) {
        int ach = 'a';
        while (ach != 'q') {
          ach = getch();
          this->section2.menu_driver(ach, ARTIST_WINDOW_INDEX);
        }
      }
    }
  }
}

/* Destructor for TUI class */
TUI::~TUI() { endwin(); }
