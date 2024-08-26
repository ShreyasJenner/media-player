/* Program file to implement a progress bar 
 * Must be called after the file providing the data for progess bar is started
 */

#include <sys/ioctl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

#include "struct.h"
#include "progress_bar.h"

// global flag to indicate terminal dimension change
volatile sig_atomic_t flag = 0;

// function gets the terminal dimensions and stores it in struct
void get_term_dim(struct winsize* ws) {
    // store current terminal dimensions in struct ws
    ioctl(0, TIOCGWINSZ, ws); 
}

// function sets flag to 1 to inform that terminal has changed dimensions
void refresh_dim(int signum) {
    flag = 1;
}

// function maps the value given into a value between 0 and term_width 
int map_into_range(int val, int term_width, int data_max) {
    return (val*term_width)/data_max;
}

// function to draw progress bar in screen
void* progress_bar(void *args) {
    key_t key;
    int shmid;
    struct dimensions *dm;

   
    key = ftok("Makefile", 100);
    if(key<0) {
        perror("using ftok");
        exit(1);
    }

    shmid = shmget(key, sizeof(struct dimensions), 0666|IPC_CREAT);
    if(shmid<0) {
        perror("Calling shmget");
        exit(1);
    }

    // attach shared memory
    dm = (struct dimensions*)shmat(shmid, NULL, 0);
    if(dm==(void *)-1) {
        perror("Attaching shared memory buffer");
        exit(1);
    }
    
    // set start to 0 to indicate data hasnt started being updated
    dm->start = 0;


    // wait till start is changed to 1
    while(dm->start==0) {;}

    // print progress bar
    struct winsize ws;
    int term_itr, i;


    term_itr=0;

    // store terminal dimensions in ws
    get_term_dim(&ws);

    // while loop to print the progress bar on the screen
    while(term_itr<ws.ws_col) {
        // check if terminal dimensions have changed
        signal(SIGWINCH, refresh_dim);
        // if the terminal dimensions have changed
        if(flag==1) {
            // get the new terminal dimensions
            get_term_dim(&ws);

            // go to the next line of the terminal
            printf("\n");

            // get the length covered by progress bar for new terminal dimensions
            term_itr = map_into_range(dm->data_cur_val, ws.ws_col, dm->data_max_val);

            // print the length covered by progress bar
            for(i=0;i<term_itr;i++)
                printf("#");
            fflush(stdout);

            // change flag back to 0
            flag = 0;
        }

        // wait in the while loop for data value to increase
        while(term_itr>map_into_range(dm->data_cur_val, ws.ws_col, dm->data_max_val)) {
        }
        printf("#");
        // flush buffer to stdout
        fflush(stdout);
        term_itr++;
    }

    // detach shared memory 
    shmdt(dm);

    // destroy shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
