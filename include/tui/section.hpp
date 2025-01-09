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

public:
  WINDOW *win;
  PANEL *panel;

  Section();

  void init(int rows, int cols, int startx, int starty);

  ~Section();
};

class Section1 : public Section {
public:
  MENU *menu;
  ITEM **items;
  WINDOW *subwin;
  int item_count;
  char options[SECTION_1_OPTIONS][OPTION_LEN];

  Section1();

  void configure_menu_settings();

  void create_menu();

  void pattern_matching();

  void menu_driver(int key);

  ~Section1();
};

#endif // !SECTION_HPP
