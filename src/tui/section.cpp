#include "tui/section.hpp"

/* Default Constructor for Section class */
Section::Section() {

  // perform initialization of class variables
  this->win = NULL;
  this->panel = NULL;
}

/* Function to create window and panel */
void Section::init(int rows, int cols, int startx, int starty) {

  // store the window dimensions into the class variables
  this->startx = startx;
  this->starty = starty;
  this->rows = rows;
  this->cols = cols;

  // create window and panel
  this->win = newwin(rows, cols, starty, startx);
  this->panel = new_panel(this->win);

  // put a box around window and refresh it
  box(this->win, 0, 0);
  wrefresh(this->win);
}

/* Destructor for Section class */
Section::~Section() {

  // delete panel and window if it exists
  if (this->panel != NULL) {
    del_panel(this->panel);
  }

  if (this->win != NULL) {
    delwin(this->win);
  }
}
