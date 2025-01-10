#include "tui/section.hpp"

/* Default Constructor for Section class */
Section::Section() {

  // perform initialization of class variables
  this->wins = NULL;
  this->panels = NULL;
}

/* Function to allocate space for window and panel arrays */
void Section::init(int win_count) {
  this->win_count = win_count;

  // allocate space for window and panels
  this->wins = (WINDOW **)calloc(win_count, sizeof(WINDOW *));
  this->panels = (PANEL **)calloc(win_count, sizeof(PANEL *));
}

/* Function to create window and panel */
void Section::init(int rows, int cols, int startx, int starty) {
  int i;

  // store the window dimensions into the class variables
  this->startx = startx;
  this->starty = starty;
  this->rows = rows;
  this->cols = cols;

  // create windows and panels; then put a box around the windows and refresh
  // them
  for (i = 0; i < this->win_count; i++) {
    this->wins[i] = newwin(rows, cols, starty, startx);
    this->panels[i] = new_panel(this->wins[i]);
    box(this->wins[i], 0, 0);
    wrefresh(this->wins[i]);
  }
}

/* Function to return pointer to window */
WINDOW **Section::get_win() { return this->wins; }

/* Function to return pointer to panel */
PANEL **Section::get_panel() { return this->panels; }

/* Destructor for Section class */
Section::~Section() {
  int i;

  // delete panels and windows if they exist
  for (i = 0; i < this->win_count; i++) {
    if (this->panels[i] != NULL) {
      del_panel(this->panels[i]);
    }

    if (this->wins[i] != NULL) {
      delwin(this->wins[i]);
    }
  }

  // free window and panel array
  free(this->wins);
  free(this->panels);
}
