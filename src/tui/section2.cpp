#include "tui/section.hpp"

/*
 * Section2 class deals with rendering the main viewer window of the media
 * player.
 * Based on the option selected in Section1, a corresponding window is rendered
 * in the Section2 window.
 * Section2 renders Artist, Album, and Search window, whose options are
 * available in the menu in Section1.
 * Other than that, Section2 also offers a
 * lyric window that can be toggled to be shown or hidden when a track is
 * playing
 */

/* Default Constructor for Section2 class */
Section2::Section2() {
  int i;

  // initialization of class variables
  for (i = 0; i < SECTION_2_WIN_COUNT; i++) {

    this->subwin[i] = NULL;
    this->items[i] = NULL;
    this->menu[i] = NULL;
  }
}

/* Function that acts as menu driver for menu */
void Section2::menu_driver(int key, int menu_index) {

  // move selection in menu
  switch (key) {
  case 'j':
    ::menu_driver(this->menu[ARTIST_WINDOW_INDEX], REQ_DOWN_ITEM);
    break;

  case 'k':
    ::menu_driver(this->menu[ARTIST_WINDOW_INDEX], REQ_UP_ITEM);
    break;

    // enter key to store selected item
  case 10:
  case KEY_ENTER:
    this->selected = current_item(this->menu[ARTIST_WINDOW_INDEX]);
  }

  // refresh the menu window
  wrefresh(this->wins[ARTIST_WINDOW_INDEX]);
}

/* Function to create window for artist option in Section1
 * The function displays a list of artists in a menu
 * Window 0 and Panel 0 deal with this window
 */
// BUG: menu does not cover full window in Section2
void Section2::create_sec1_artist_window(std::vector<Node *> artist_ptrs) {

  int i;

  // allocate space for the item array
  this->item_count[ARTIST_WINDOW_INDEX] = artist_ptrs.size();

  this->items[ARTIST_WINDOW_INDEX] = (ITEM **)calloc(
      this->item_count[ARTIST_WINDOW_INDEX] + 1, sizeof(ITEM *));

  // populate the item array
  for (i = 0; i < this->item_count[ARTIST_WINDOW_INDEX]; i++) {
    this->items[ARTIST_WINDOW_INDEX][i] = new_item(
        artist_ptrs[i]->mbfilename.c_str(), artist_ptrs[i]->mbpath.c_str());
  }
  this->items[ARTIST_WINDOW_INDEX][this->item_count[ARTIST_WINDOW_INDEX]] =
      NULL;

  // create the menu and configure it
  this->menu[ARTIST_WINDOW_INDEX] = new_menu(this->items[ARTIST_WINDOW_INDEX]);

  // configure the settings for the artist list menu
  this->subwin[ARTIST_WINDOW_INDEX] = derwin(
      this->wins[ARTIST_WINDOW_INDEX], this->rows - 1, this->cols - 2, 1, 1);
  set_menu_win(this->menu[ARTIST_WINDOW_INDEX],
               this->wins[ARTIST_WINDOW_INDEX]);
  set_menu_sub(this->menu[ARTIST_WINDOW_INDEX],
               this->subwin[ARTIST_WINDOW_INDEX]);

  // configure menu settings
  set_menu_mark(this->menu[ARTIST_WINDOW_INDEX], " * ");
  menu_opts_on(this->menu[ARTIST_WINDOW_INDEX], O_ONEVALUE);
  menu_opts_off(this->menu[ARTIST_WINDOW_INDEX], O_SHOWDESC);
  menu_opts_off(this->menu[ARTIST_WINDOW_INDEX], O_NONCYCLIC);

  post_menu(this->menu[ARTIST_WINDOW_INDEX]);
  wrefresh(this->wins[ARTIST_WINDOW_INDEX]);
}

/* Function to show window corresponding to selected item in Section1 */
void Section2::render_selected_win(const char *selected) {

  // NOTE: The selected item name can correspond to one of three following
  // values:
  // 1. Artist
  // 2. Album
  // 3. Search
  // The corresponding window related to selected item name should be shown
  if (!strcmp(selected, "Artist")) {

    top_panel(this->get_panel()[0]);
  } else if (!strcmp(selected, "Album")) {
    // NOTE: temp code to show that album window is rendered
    mvwprintw(this->get_win()[1], 1, 1, "Album Window");
    top_panel(this->get_panel()[1]);
  } else if (!strcmp(selected, "Search")) {
    // NOTE: temp code to show that search window is rendered
    mvwprintw(this->get_win()[2], 1, 1, "Search Window");
    top_panel(this->get_panel()[2]);
  }

  update_panels();
  doupdate();
}

/* Destructor for Section2 class */
Section2::~Section2() {
  int i, j;

  // free resources
  for (i = 0; i < SECTION_2_WIN_COUNT; i++) {

    // free the items, corresponding menu and sub-window
    if (this->items[i] != NULL) {
      for (j = 0; j < this->item_count[i]; j++) {
        free_item(this->items[i][j]);
      }
      free(this->items[i]);
    }

    if (this->menu[i] != NULL) {
      free_menu(this->menu[i]);
    }

    if (this->subwin[i] != NULL) {
      delwin(this->subwin[i]);
    }
  }
}
