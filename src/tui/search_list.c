#include "format.h"
#include "log.h"
#include "stdheader.h"
#include "structs.h"
#include "tree.h"
#include "tui/search_list.h"
#include <ncurses.h>

/*
 * Function gets called to search the list of files
 * It calls the search_Tree function to get a list of matching
 * files
 * A single user selected file is copied into selected_file which
 * can be used by calling function
 * Returns number of matches to the input pattern
 */
int search_MusicList(struct Tree tree, char *selected_file) {
  /* Declaration */
  int matches, ch, max_filename_length, i, padding, pad_ht, pad_width,
      pad_start_x, pad_start_y, pad_visible_width, pad_visible_height,
      horizontal_offset, vertical_offset;
  char pattern[FILE_NAME_SZ];
  char filename[FILE_NAME_SZ];
  char choice[tree.filecount + 1];
  char filelist[tree.filecount][FILE_NAME_SZ];

  WINDOW *pad;
  MEVENT event;

  /* set getch to blocking and enable echo */
  nodelay(stdscr, false);
  echo();

  /* store no of matches to input pattern */
  printw("Enter file name:");
  getstr(pattern);
  matches = search_Tree(tree, pattern, filelist);

  /* get length of longest file name */
  max_filename_length = 0;
  for (i = 0; i < matches; i++) {
    if (strlen(filelist[i]) > max_filename_length)
      max_filename_length = strlen(filelist[i]);
  }

  /* Account for the index of file and : */
  padding = 3;

  /* Initialize pad */
  pad_ht = matches;
  /* +3 to account for printing index of files */
  pad_width = max_filename_length + padding;
  pad_start_y = 0;
  pad_start_x = (getmaxx(stdscr) / 4) - padding;
  /* +3 to account for printing index of files */
  pad_visible_width = (3 * (getmaxx(stdscr) / 4)) + padding;
  pad_visible_height = 3 * (getmaxy(stdscr) / 4);
  vertical_offset = 0;
  horizontal_offset = 0;

  /* create pad and enable scrolling */
  pad = newpad(pad_ht, pad_width);
  scrollok(pad, true);

  /* print list of files that match pattern into pad and show pad */
  wmove(pad, 0, 0);
  for (i = 0; i < matches; i++) {
    // wprintw(pad,"%d:%s",i,filelist[i]);
    get_FileName(filelist[i], filename);
    wprintw(pad, "%d:%s", i, filename);
    wmove(pad, i + 1, 0);
  }
  prefresh(pad,
           /* starting co-ordinates of pad */
           0, 0,
           /* upper left corner in main window */
           pad_start_y, pad_start_x,
           /* lower right corner in main window */
           pad_start_y + pad_visible_height - 1,
           pad_start_x + pad_visible_width - 1);

  /* while scrolling stay in loop */
  nodelay(stdscr, true);
  while (true) {
    ch = getch();

    /* if mouse event detected */
    if (ch == KEY_MOUSE) {
      if (getmouse(&event) == OK) {

        if (event.bstate & BUTTON4_PRESSED) {
          /* vertical scrolling up */
          if (vertical_offset > 0)
            --vertical_offset;
          prefresh(pad, vertical_offset, horizontal_offset, pad_start_y,
                   pad_start_x, pad_start_y + pad_visible_height - 1,
                   pad_start_x + pad_visible_width - 1);
        } else if (event.bstate & BUTTON5_PRESSED) {
          /* vertical scrolling down */
          if (vertical_offset < matches - pad_visible_height)
            ++vertical_offset;
          prefresh(pad, vertical_offset, horizontal_offset, pad_start_y,
                   pad_start_x, pad_start_y + pad_visible_height - 1,
                   pad_start_x + pad_visible_width - 1);
        }
      }
    } else if (ch == KEY_LEFT) {
      /* horizontal scrolling left */
      if (horizontal_offset > 0)
        --horizontal_offset;
      prefresh(pad, vertical_offset, horizontal_offset, pad_start_y,
               pad_start_x, pad_start_y + pad_visible_height - 1,
               pad_start_x + pad_visible_width - 1);
    } else if (ch == KEY_RIGHT) {
      /* horizontal scrolling right */
      if (horizontal_offset < pad_width - pad_visible_width)
        ++horizontal_offset;
      prefresh(pad, vertical_offset, horizontal_offset, pad_start_y,
               pad_start_x, pad_start_y + pad_visible_height - 1,
               pad_start_x + pad_visible_width - 1);
    } else if (ch == 'q') {
      break; // Exit on 'q' key press
    }
  }
  /* set no delay back to false */
  nodelay(stdscr, false);

  /* get file selection from user */
  move(pad_visible_height, 0);
  printw("Enter file index:");
  getstr(choice);
  refresh();

  /* convert string input to integer and copy selected file */
  ch = strtol(choice, NULL, 10);
  printw("%d\n", ch);
  strcpy(selected_file, filelist[ch]);
  printw("Selected file:%s\n", selected_file);
  refresh();

  /* clear the screen after displaying choice */
  usleep(100 * 1000);
  clear();

  /* set getch to non-blocking */
  nodelay(stdscr, true);
  noecho();

  return matches;
}
