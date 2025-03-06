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
    ::menu_driver(this->menu[menu_index], REQ_DOWN_ITEM);
    break;

  case 'k':
    ::menu_driver(this->menu[menu_index], REQ_UP_ITEM);
    break;

    // enter key to store selected item
  case 10:
  case KEY_ENTER:
    this->selected = current_item(this->menu[menu_index]);
  }

  // refresh the menu window
  wrefresh(this->wins[menu_index]);
}

/* Function to create window for artist option in Section1
 * The function displays a list of artists in a menu
 * Window 0 and Panel 0 deal with this window
 */
void Section2::create_sec1_artist_window(std::vector<Node *> artist_ptrs) {

  int i;

  // allocate space for the item array
  this->item_count[ARTIST_WINDOW_INDEX] = artist_ptrs.size();

  this->items[ARTIST_WINDOW_INDEX] = (ITEM **)calloc(
      this->item_count[ARTIST_WINDOW_INDEX] + 1, sizeof(ITEM *));

  // populate the item array and associate the corresponding node ptr with the
  // item ptr
  for (i = 0; i < this->item_count[ARTIST_WINDOW_INDEX]; i++) {
    this->items[ARTIST_WINDOW_INDEX][i] = new_item(
        artist_ptrs[i]->mbfilename.c_str(), artist_ptrs[i]->mbpath.c_str());
    set_item_userptr(this->items[ARTIST_WINDOW_INDEX][i], artist_ptrs[i]);
  }
  this->items[ARTIST_WINDOW_INDEX][this->item_count[ARTIST_WINDOW_INDEX]] =
      NULL;

  // create the menu and configure it
  this->menu[ARTIST_WINDOW_INDEX] = new_menu(this->items[ARTIST_WINDOW_INDEX]);

  // configure the settings for the artist list menu
  this->subwin[ARTIST_WINDOW_INDEX] = derwin(
      this->wins[ARTIST_WINDOW_INDEX], this->rows - 3, this->cols - 2, 2, 1);
  set_menu_win(this->menu[ARTIST_WINDOW_INDEX],
               this->wins[ARTIST_WINDOW_INDEX]);
  set_menu_sub(this->menu[ARTIST_WINDOW_INDEX],
               this->subwin[ARTIST_WINDOW_INDEX]);

  // put "Artists" title for menu
  wattron(this->wins[ARTIST_WINDOW_INDEX], A_BOLD | A_UNDERLINE);
  mvwprintw(this->wins[ARTIST_WINDOW_INDEX], 1, 4, "%s", "Artists");
  wattroff(this->wins[ARTIST_WINDOW_INDEX], A_BOLD | A_UNDERLINE);

  // configure menu settings
  set_menu_mark(this->menu[ARTIST_WINDOW_INDEX], " * ");
  set_menu_format(this->menu[ARTIST_WINDOW_INDEX], this->rows - 3, 1);
  menu_opts_on(this->menu[ARTIST_WINDOW_INDEX], O_ONEVALUE);
  menu_opts_off(this->menu[ARTIST_WINDOW_INDEX], O_SHOWDESC);
  menu_opts_off(this->menu[ARTIST_WINDOW_INDEX], O_NONCYCLIC);

  post_menu(this->menu[ARTIST_WINDOW_INDEX]);
  wrefresh(this->wins[ARTIST_WINDOW_INDEX]);
}

/* Function to populate the discography window for a selected artist
 * The function is called when an artist is selected from the artist window
 * It displays the list of albums that the artist has created
 */
void Section2::populate_sec1_artist_discog_window(Node *artist_ptr) {
  int i;
  Node *ptr;

  // set ptr to first album in artist discography
  i = 0;
  this->item_count[ARTIST_DISCOG_INDEX] = 0;
  ptr = artist_ptr->child;

  // allocate space for artist album array
  while (ptr != NULL) {
    this->item_count[ARTIST_DISCOG_INDEX]++;
    ptr = ptr->sibling;
  }

  this->items[ARTIST_DISCOG_INDEX] = (ITEM **)calloc(
      this->item_count[ARTIST_DISCOG_INDEX] + 1, sizeof(ITEM *));

  // populate the item array and associate the corresponding node ptr with the
  // item ptr
  ptr = artist_ptr->child;
  for (i = 0; i < this->item_count[ARTIST_DISCOG_INDEX]; i++) {
    this->items[ARTIST_DISCOG_INDEX][i] =
        new_item(ptr->mbfilename.c_str(), ptr->mbpath.c_str());
    ptr = ptr->sibling;
  }
  this->items[ARTIST_DISCOG_INDEX][this->item_count[ARTIST_DISCOG_INDEX]] =
      NULL;

  // create the menu and configure it
  this->menu[ARTIST_DISCOG_INDEX] = new_menu(this->items[ARTIST_DISCOG_INDEX]);

  // configure the settings for the artist discography menu
  this->subwin[ARTIST_DISCOG_INDEX] = derwin(
      this->wins[ARTIST_DISCOG_INDEX], this->rows - 3, this->cols - 2, 2, 1);
  set_menu_win(this->menu[ARTIST_DISCOG_INDEX],
               this->wins[ARTIST_DISCOG_INDEX]);
  set_menu_sub(this->menu[ARTIST_DISCOG_INDEX],
               this->subwin[ARTIST_DISCOG_INDEX]);

  // put name of artist as title for menu
  wattron(this->wins[ARTIST_DISCOG_INDEX], A_BOLD | A_UNDERLINE);
  mvwprintw(this->wins[ARTIST_DISCOG_INDEX], 1, 4, "%s",
            artist_ptr->mbfilename.c_str());
  wattroff(this->wins[ARTIST_DISCOG_INDEX], A_BOLD | A_UNDERLINE);

  // configure menu settings
  set_menu_mark(this->menu[ARTIST_DISCOG_INDEX], " * ");
  set_menu_format(this->menu[ARTIST_DISCOG_INDEX], this->rows - 3, 1);
  menu_opts_on(this->menu[ARTIST_DISCOG_INDEX], O_ONEVALUE);
  menu_opts_off(this->menu[ARTIST_DISCOG_INDEX], O_SHOWDESC);
  menu_opts_off(this->menu[ARTIST_DISCOG_INDEX], O_NONCYCLIC);

  post_menu(this->menu[ARTIST_DISCOG_INDEX]);
  wrefresh(this->wins[ARTIST_DISCOG_INDEX]);
}

/* Function to depopulate menu entries in artist discography window
 * Must be called after menu populated by populate_sec1_artist_discog_window is
 * no longer needed
 */
void Section2::depopulate_sec1_artist_discog_window() {
  int i;

  // free the artist discography entries
  for (i = 0; i < this->item_count[ARTIST_DISCOG_INDEX]; i++) {
    free_item(this->items[ARTIST_DISCOG_INDEX][i]);
  }
  free(this->items[ARTIST_DISCOG_INDEX]);

  // free the menu
  free_menu(this->menu[ARTIST_DISCOG_INDEX]);

  // free the sub-window
  delwin(this->subwin[ARTIST_DISCOG_INDEX]);
}

/* Function to create album window for Section1 album option
 * The window displays a list of albums that can be selected and opened to see
 * the track list in the album
 */
void Section2::create_sec1_album_window(std::vector<Node *> album_ptrs) {
  int i;

  // allocate space for the item array
  this->item_count[ALBUM_WINDOW_INDEX] = album_ptrs.size();

  this->items[ALBUM_WINDOW_INDEX] =
      (ITEM **)calloc(this->item_count[ALBUM_WINDOW_INDEX] + 1, sizeof(ITEM *));

  // populate the item array and associate the corresponding node ptr with the
  // item ptr
  for (i = 0; i < this->item_count[ALBUM_WINDOW_INDEX]; i++) {
    this->items[ALBUM_WINDOW_INDEX][i] = new_item(
        album_ptrs[i]->mbfilename.c_str(), album_ptrs[i]->mbpath.c_str());
    set_item_userptr(this->items[ALBUM_WINDOW_INDEX][i], album_ptrs[i]);
  }
  this->items[ALBUM_WINDOW_INDEX][this->item_count[ALBUM_WINDOW_INDEX]] = NULL;

  // create the menu and configure it
  this->menu[ALBUM_WINDOW_INDEX] = new_menu(this->items[ALBUM_WINDOW_INDEX]);

  // configure the settings for the album list menu
  this->subwin[ALBUM_WINDOW_INDEX] = derwin(
      this->wins[ALBUM_WINDOW_INDEX], this->rows - 3, this->cols - 2, 2, 1);
  set_menu_win(this->menu[ALBUM_WINDOW_INDEX], this->wins[ALBUM_WINDOW_INDEX]);
  set_menu_sub(this->menu[ALBUM_WINDOW_INDEX],
               this->subwin[ALBUM_WINDOW_INDEX]);

  // put "Albums" title for menu
  wattron(this->wins[ALBUM_WINDOW_INDEX], A_BOLD | A_UNDERLINE);
  mvwprintw(this->wins[ALBUM_WINDOW_INDEX], 1, 4, "%s", "Albums");
  wattroff(this->wins[ALBUM_WINDOW_INDEX], A_BOLD | A_UNDERLINE);

  // configure menu settings
  set_menu_mark(this->menu[ALBUM_WINDOW_INDEX], " * ");
  set_menu_format(this->menu[ALBUM_WINDOW_INDEX], this->rows - 3, 1);
  menu_opts_on(this->menu[ALBUM_WINDOW_INDEX], O_ONEVALUE);
  menu_opts_off(this->menu[ALBUM_WINDOW_INDEX], O_SHOWDESC);
  menu_opts_off(this->menu[ALBUM_WINDOW_INDEX], O_NONCYCLIC);

  post_menu(this->menu[ALBUM_WINDOW_INDEX]);
  wrefresh(this->wins[ALBUM_WINDOW_INDEX]);
}

/* Function to create window that will allow search operations
 * The function offers a search bar that will return all results that match the
 * pattern Results Filtering such as only artists, only albums, only Tracks,
 * etc. can be done
 */
// TODO: implement the function
void Section2::create_sec1_search_window(Trie *tr) {}

/* Function to show window corresponding to selected item in Section1 */
void Section2::render_selected_win(const char *selected) {

  // NOTE: The selected item name can correspond to one of following
  // values:
  // 1. Artist
  // 2. Album
  // 3. Search
  // The corresponding window related to selected item name should be shown
  if (!strcmp(selected, "Artist")) {

    top_panel(this->get_panel()[ARTIST_WINDOW_INDEX]);
  } else if (!strcmp(selected, "Album")) {

    top_panel(this->get_panel()[ALBUM_WINDOW_INDEX]);
  } else if (!strcmp(selected, "Search")) {
    // NOTE: temp code to show that search window is rendered
    mvwprintw(this->get_win()[SEARCH_WINDOW_INDEX], 1, 1, "Search Window");
    top_panel(this->get_panel()[SEARCH_WINDOW_INDEX]);
  }

  update_panels();
  doupdate();
}

/* Function to return a pointer to the selected item in the menu */
ITEM *Section2::get_selected_item() { return this->selected; }

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
