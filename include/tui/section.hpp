#ifndef SECTION_HPP
#define SECTION_HPP

#define SECTION_1_OPTIONS 3
#define OPTION_LEN 7

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

  WINDOW *win;
  PANEL *panel;

public:
  Section();

  void init(int rows, int cols, int startx, int starty);

  WINDOW *get_win();

  PANEL *get_panel();

  ~Section();
};

class Section1 : public Section {
private:
  int item_count;
  char options[SECTION_1_OPTIONS][OPTION_LEN];

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

#endif // !SECTION_HPP
