#include "tui/section.hpp"

/* Default Constructor for Section1 class */
Section1::Section1() {

  // perform initialization of class variables
  this->menu = NULL;
  this->selected = NULL;
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

    // enter key to store selected item
  case 10:
  case KEY_ENTER:
    this->selected = current_item(this->menu);
  }

  // refresh the menu window
  wrefresh(this->win);
}

/* Function to return pointer to selected item in menu */
ITEM *Section1::get_selected_item() { return this->selected; }

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
