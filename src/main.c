/* Main Program file */

/* header files */
#include "interfaces/ss_mp.h"
#include "log.h"
#include "media-handling/sdl.h"
#include "mp3_lut.h"
#include "stdheader.h"
#include "structs.h"
#include "tree.h"
#include "tui/init.h"
#include "tui/search_list.h"
#include "tui/start_screen.h"

/* external header files */
#include "id3reader/include/id3reader.h"
#include "progress-bar/include/progress_bar.h"
#include <curses.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  /* check if file arg has been passed */
  if (argc < 2) {
    logerror(__func__, "File argument not passed");
    return 0;
  }

  /* Declaration */
  int result;
  char sel_file[FILE_NAME_SZ];
  struct Tree tree;

  /* initalize ncurses */
  tui_init();

  /* get dir tree */
  result = get_DirTree(&tree, argv[1]);
  if (result != 0) {
    logerror(__func__, "Calling get_DirTree");
    return 0;
  }

  // NOTE: temp code to check start screen
  char filelist[tree.root.childcount][2][FILE_NAME_SZ];
  int ch;
  PANEL *panel;
  MENU *menu;

  // interface_ss_mp(&tree.root, filelist);
  interface_ss_mp(&tree.root, filelist);
  menu = start_screen_init(&panel, filelist, tree.root.childcount);
  update_panels();
  doupdate();

  while ((ch = getch()) != 'q') {
    handle_keypress(menu, ch);
    update_panels();
    doupdate();
  }
  del_panel(panel);
  start_screen_deinit(menu);
  // NOTE: end of temp code

  /* search tree for matching list */
  search_MusicList(tree, sel_file);

  /* initalize sdl mixer */
  if (sdl_init() != 0) {
    logerror(__func__, "Error in function sdl_init");
    endwin();
    exit(1);
  }

  /* load the music file */
  Mix_Music *music = load_music(sel_file);

  // create a thread to detect key presses
  // thread_id = pthread_create(&thread_id, NULL, key_detect, &music);

  // TODO: external thread should support key handling
  int flag;
  flag = 1;
  // BUG: fix cursor flickering
  // NOTE: temprary code to fix cursor flickering
  curs_set(0);
  // NOTE: temprary code to fix cursor flickering
  /* Loop deals with playing, pausing, rewinding and quitting music player */
  while (true) {
    // music is over
    if (!Mix_PlayingMusic())
      break;

    ch = getch();

    flag = sdl_HandleKeypress(music, ch, flag);
    if (flag == MEDIA_PLAYER_QUIT)
      break;

    /* Print the current playback position */
    // mvprintw(0, 0, "%d\n", (int)Mix_GetMusicPosition(music));
    //
    //
    // NOTE: progress bar to the right is not cleared if rewinding of music is
    // done
    // music position printed is not cleared; remnants are left if a smaller
    // number is printed after a larger one
    // BUG: fix progress_bar or media player to handle artifacts left behind
    // when rewinding music
    // NOTE: temporary code to handle bug
    move(0, 0);
    clrtoeol();
    move(1, 0);
    clrtoeol();
    // NOTE: temporary code to handle bug
    // TODO: modify progress_bar to show only the progress_bar and not print the
    // data value passed to it
    progress_bar((int)Mix_GetMusicPosition(music),
                 (int)Mix_MusicDuration(music), 0);
    wrefresh(stdscr);
  }

  // block program until quit is pressed
  // pthread_join(thread_id, NULL);

  /* Deinitialize sdl */
  sdl_deinit(music);

  /* close ncurses window */
  endwin();

  // NOTE: Temp code
  free_Tree(&tree.root);
  // NOTE: Temp code

  return 0;
}
