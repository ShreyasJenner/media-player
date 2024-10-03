/* Program file initializes ncurses screen */

// NOTE: global
#include "stdheader.h"
// NOTE: local
#include "log.h"
#include "tui/init.h"

/* Function initializes ncurses screen */
void tui_init() {

  setlocale(LC_ALL, "");
  initscr();
  noecho();
  raw();
  curs_set(0);
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL);
}

/* Function to create windows with respective dimensions */
WINDOW **create_windows() {
  /* Declaration */
  WINDOW **wins;

  int music_dir_x, music_dir_y, music_files_x, music_files_y, progress_bar_x,
      progress_bar_y;
  int music_dir_width, music_dir_height, music_files_width, music_files_height,
      progress_bar_height, progress_bar_width;
  int max_screen_x, max_screen_y;

  /* Initialization */
  getmaxyx(stdscr, max_screen_y, max_screen_x);

  /* Allocate space for windows */
  wins = calloc(PANEL_COUNT, sizeof(WINDOW *));
  if (wins == NULL) {
    logerror(__FILE__, __LINE__, __func__, "Error -> mallocing window array");
    return NULL;
  }

  /* set the dimensions for the 4 panel windows */
  music_dir_y = 0;
  music_dir_x = 0;
  music_dir_height = max_screen_y;
  music_dir_width = max_screen_x / 3;

  music_files_y = 0;
  music_files_x = (max_screen_x / 3) - 1;
  music_files_height = (2 * max_screen_y) / 3;
  music_files_width = max_screen_x - music_files_x;

  progress_bar_y = ((2 * max_screen_y) / 3) - 1;
  progress_bar_x = (max_screen_x / 3) - 1;
  progress_bar_height = max_screen_y - progress_bar_y;
  progress_bar_width = max_screen_x - progress_bar_x;

  /* create new windows with needed dimensions */
  wins[0] = newwin(music_dir_height, music_dir_width, music_dir_y, music_dir_x);
  wins[1] = newwin(music_files_height, music_files_width, music_files_y,
                   music_files_x);
  wins[2] = newwin(music_files_height, music_files_width, music_files_y,
                   music_files_x);
  wins[3] = newwin(progress_bar_height, progress_bar_width, progress_bar_y,
                   progress_bar_x);
  if (wins[0] == NULL || wins[1] == NULL || wins[2] == NULL ||
      wins[3] == NULL) {
    fprintf(stderr, "music_dir_y:%d\n", music_dir_y);
    fprintf(stderr, "music_dir_x:%d\n", music_dir_x);
    fprintf(stderr, "music_dir_height:%d\n", music_dir_height);
    fprintf(stderr, "music_dir_width:%d\n", music_dir_width);

    fprintf(stderr, "music_files_y:%d\n", music_files_y);
    fprintf(stderr, "music_files_x:%d\n", music_files_x);
    fprintf(stderr, "music_files_height:%d\n", music_files_height);
    fprintf(stderr, "music_files_width:%d\n", music_files_width);

    fprintf(stderr, "progress_bar_y:%d\n", progress_bar_y);
    fprintf(stderr, "progress_bar_x:%d\n", progress_bar_y);
    fprintf(stderr, "progress_bar_height:%d\n", progress_bar_height);
    fprintf(stderr, "progress_bar_width:%d\n", progress_bar_width);

    logerror(__FILE__, __LINE__, __func__, "Error creating windows");
    return NULL;
  }

  return wins;
}

/* Function to box windows and ensure that no breaks are present in the box
 * borders */
void box_windows(WINDOW **wins) {
  /* Declaration */
  int i, progress_bar_width;

  /* Initialization */
  progress_bar_width = getmaxx(wins[4]);

  /* set borders around all the windows */
  for (i = 0; i < PANEL_COUNT; i++) {
    box(wins[i], 0, 0);
  }

  /* make all boxes connect to each other so that no gaps show between windows
   */
  mvwaddch(wins[1], 0, 0, ACS_TTEE);
  mvwaddch(wins[2], 0, 0, ACS_TTEE);
  mvwaddch(wins[3], 0, 0, ACS_LTEE);
  mvwaddch(wins[3], 0, progress_bar_width - 1, ACS_RTEE);
}

/* Function creates a list of panels and windows with needed dimensions
 * 3 panels are created;
 * 1st panel -> music dir list
 * 2nd panel -> current dir music file
 * 3rd panel -> overlaps 2nd panel with lyrics
 * 4th paenel -> progress bar
 */
PANEL **create_panels(WINDOW **wins) {
  /* Declaration */
  PANEL **panels;

  /* Allocate space for panels */
  panels = calloc(PANEL_COUNT, sizeof(PANEL *));
  if (panels == NULL) {
    logerror(__FILE__, __LINE__, __func__, "Error -> mallocing panel array");
    return NULL;
  }

  /* attach windows to panels */
  panels[0] = new_panel(wins[0]);
  panels[1] = new_panel(wins[1]);
  panels[2] = new_panel(wins[2]);
  panels[3] = new_panel(wins[3]);
  if (panels[0] == NULL || panels[1] == NULL || panels[2] == NULL ||
      panels[3] == NULL) {
    logerror(__FILE__, __LINE__, __func__, "Error creating panels");
    return NULL;
  }

  return panels;
}

/* Function to destroy windows */
void destroy_windows(WINDOW **wins) {
  /* Declaration */
  int i;

  /* free windows */
  for (i = 0; i < PANEL_COUNT; i++) {
    delwin(wins[i]);
  }
  free(wins);
}

/* Function to destroy panels */
void destroy_panels(PANEL **panels) {
  /* Declaration */
  int i;

  /* free panel items and corresponding windows */
  for (i = 0; i < PANEL_COUNT; i++) {
    del_panel(panels[i]);
  }
  free(panels);
}

/* Function to update panels */
void tui_update(WINDOW **wins) {
  box_windows(wins);
  update_panels();
  doupdate();
}

/* Function to end curses */
void tui_deinit() { endwin(); }
