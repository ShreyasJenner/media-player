#include "tui/section.hpp"

/* Default Constructor for Section1 class */
Section1::Section1() {

  // perform initialization of class variables
  this->menu = NULL;
  this->items = NULL;
  this->item_count = 0;

  // populate the string options array
  strcpy(options[0], "Artist");
  strcpy(options[1], "Album");
  strcpy(options[2], "Search");
}

/* Function to configure menu */
void Section1::configure_menu_settings() {

  // set menu window and sub window
  this->subwin = derwin(this->win, 4, this->cols - 1, 1, 1);
  set_menu_win(this->menu, this->win);
  set_menu_sub(this->menu, this->subwin);

  // configure menu settings
  set_menu_mark(this->menu, " * ");
  menu_opts_on(this->menu, O_ONEVALUE);
  menu_opts_on(this->menu, O_IGNORECASE);
  menu_opts_on(this->menu, O_SHOWMATCH);
  menu_opts_off(this->menu, O_SHOWDESC);
  menu_opts_off(this->menu, O_NONCYCLIC);
}

/* Function to create and populate the section 1 menu */
void Section1::create_menu() {
  int i;

  // allocate space for the item array
  this->item_count = SECTION_1_OPTIONS;
  this->items = (ITEM **)calloc(this->item_count + 1, sizeof(ITEM *));

  // populate the item array
  for (i = 0; i < this->item_count; i++) {
    this->items[i] = new_item(this->options[i], this->options[i]);
  }
  this->items[this->item_count] = NULL;

  // create the menu and configure it
  this->menu = new_menu(this->items);
  this->configure_menu_settings();

  post_menu(this->menu);
  wrefresh(this->win);
}

/* Function to handle pattern matching in menu */
// TODO: modify this function according to the requirements of the next todo
// when the function is triggered a window at the bottom of section1 should show
// the entered characters
void Section1::pattern_matching() {
  int ch;

  ch = 'a';

  // exit the loop if escape is pressed
  while (ch != 27) {
    ch = getch();

    // switch case to handle entering of values in pattern matching
    switch (ch) {

    // if escape is pressed remove a character from the pattern buffer
    case KEY_BACKSPACE:
    case 127:
    case '\b':
      ::menu_driver(this->menu, REQ_BACK_PATTERN);
      break;

      // else add the character to the pattern buffer
    default:
      ::menu_driver(this->menu, ch);
    }

    // TODO: add a scrollable window that shows the entered pattern
    //  the window should handle backspace events and filling events
    //  print the menu pattern to the bottom of the window and refresh the
    //  window
    mvwprintw(this->win, this->rows - 2, 1, "%s", menu_pattern(this->menu));
    wrefresh(this->win);
  }
  ::menu_driver(this->menu, REQ_NEXT_MATCH);

  // clear the menu pattern buffer
  ::menu_driver(this->menu, REQ_CLEAR_PATTERN);
}

/* Function that acts as menu driver for menu */
void Section1::menu_driver(int key) {

  // move selection in menu
  switch (key) {
  case 'j':
    ::menu_driver(this->menu, REQ_DOWN_ITEM);
    break;

  case 'k':
    ::menu_driver(this->menu, REQ_UP_ITEM);
    break;

    // pattern matching
  case '/':
    this->pattern_matching();
    break;

    // TODO: add support for mouse key pressing
  }

  // refresh the menu window
  wrefresh(this->win);
}

/* Destructor for Section1 class */
Section1::~Section1() {
  int i;

  // free the items and the menu
  for (i = 0; i < this->item_count; i++) {
    if (this->items[i] != NULL) {
      free_item(this->items[i]);
    }
  }

  if (this->menu != NULL) {
    free_menu(this->menu);
  }

  if (this->subwin != NULL) {
    delwin(this->subwin);
  }
}
