#ifndef SECTION_HPP
#define SECTION_HPP

#define SECTION_1_OPT_COUNT 3
#define OPTION_LEN 7

#define SECTION_2_WIN_COUNT 5
#define ARTIST_WINDOW_INDEX 0
#define ALBUM_WINDOW_INDEX 1
#define SEARCH_WINDOW_INDEX 2
#define LYRIC_WINDOW_INDEX 3
#define ARTIST_DISCOG_INDEX 4

#include "FileSystem/FileSystem.hpp"

extern "C" {
#include <menu.h>
#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>
}

class Section {
protected:
  int startx, starty, rows, cols;
  int win_count;

  WINDOW **wins;
  PANEL **panels;

public:
  Section();

  void init(int win_count);

  void init(int rows, int cols, int startx, int starty);

  WINDOW **get_win();

  PANEL **get_panel();

  ~Section();
};

class Section1 : public Section {
private:
  int item_count;
  char options[SECTION_1_OPT_COUNT][OPTION_LEN];

  WINDOW *subwin;
  ITEM *selected;
  ITEM **items;
  MENU *menu;

public:
  Section1();

  void configure_menu_settings();

  void create_menu();

  void menu_driver(int key);

  ITEM *get_selected_item();

  ~Section1();
};

class Section2 : public Section {
private:
  int item_count[SECTION_2_WIN_COUNT];

  WINDOW *subwin[SECTION_2_WIN_COUNT];
  ITEM *selected;
  ITEM **items[SECTION_2_WIN_COUNT];
  MENU *menu[SECTION_2_WIN_COUNT];

public:
  Section2();

  void menu_driver(int key, int menu_index);

  void create_sec1_artist_window(std::vector<Node *> artist_ptrs);

  void create_sec1_artist_discog_window();

  void populate_sec1_artist_discog_window(Node *artist_ptr);

  void depopulate_sec1_artist_discog_window();

  void render_selected_win(const char *selected);

  ITEM *get_selected_item();

  ~Section2();
};

#endif // !SECTION_HPP
