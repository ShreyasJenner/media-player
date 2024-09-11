#include "stdheader.h"
#include "structs.h"
#include "tree.h"
#include "tui.h"
#include "log.h"
#include <ncurses.h>
#include <regex.h>
#include <stdlib.h>
#include <unistd.h>


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
    int matches, ch, pad_scroll_y,
        pad_ht, pad_width, 
        pad_start_x, pad_start_y, 
        pad_end_x, pad_end_y;
    char pattern[FILE_NAME_SZ];
    char choice[tree.filecount+1];
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
 


    /* Initialize pad */
    pad_ht = matches;
    pad_width = 3*(getmaxx(stdscr)/4);
    pad_start_y = 0;
    pad_start_x = getmaxx(stdscr)/4;
    pad_end_x = getmaxx(stdscr);
    pad_end_y = 3*(getmaxy(stdscr)/4);


    /* create pad and enable scrolling */
    pad = newpad(pad_ht, pad_width);
    scrollok(pad, true);

    
    /* print list of files that match pattern into pad and show pad */
    wmove(pad, 0, 0);
    for(int i=0;i<matches;i++) {
        wprintw(pad,"%d:%s",i,filelist[i]);
        wmove(pad, i+1, 0);
    }
    prefresh(pad, 
            /* starting co-ordinates of pad */
            0, 0, 
            /* upper left corner in main window */
            pad_start_y, pad_start_x, 
            /* lower right corner in main window */
            pad_end_y-1, pad_end_x-1);

    pad_scroll_y = 0;
    /* while scrolling stay in loop */
    nodelay(stdscr, true);
    while(true) {
        ch = getch();   // Wait for user input

        /* if scrolling is begin done scroll the pad */
        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON4_PRESSED) {
                    /* decrement pad_scroll_y only if within screen limits */
                    if(pad_scroll_y>0)
                        --pad_scroll_y;
                    prefresh(pad, 
                            pad_scroll_y, 0, 
                            pad_start_y, pad_start_x, 
                            pad_end_y-1, pad_end_x-1);
                } else if (event.bstate & BUTTON5_PRESSED) {
                    if(pad_scroll_y < matches-pad_end_y)
                        ++pad_scroll_y;
                    prefresh(pad, 
                            pad_scroll_y, 0, 
                            pad_start_y, pad_start_x, 
                            pad_end_y-1, pad_end_x-1);
                }
            }
        } else if (ch == 'q') {
            break;          // Exit on 'q' key press
        }
    }
    /* set no delay back to false */
    nodelay(stdscr, false);

    /* get file selection from user */
    move(pad_end_y, 0);
    printw("Enter file index:");
    getstr(choice);
    refresh();

    /* convert string input to integer and copy selected file */
    ch = strtol(choice, NULL, 10);
    printw("%d\n",ch);
    strcpy(selected_file, filelist[ch]);
    printw("Selected file:%s\n",selected_file);
    refresh();

    /* clear the screen after displaying choice */
    usleep(100*1000);
    clear();

    /* set getch to non-blocking */
    nodelay(stdscr, true);

    return  matches;
}
