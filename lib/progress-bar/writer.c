/* Program file to implement writer code for writing into shared memory buffer
 * Must be called after the progess bar executable is started
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "writer.h"

struct dimensions* init(int data_max_val) {
    key_t key;
    int shmid;
    struct dimensions *dm;

    // generate a key
    key = ftok("Makefile", 100);
    if(key==-1) {
        perror("using ftok");
        exit(1);
    }

    // get shmid
    shmid = shmget(key, sizeof(struct dimensions), 0666|IPC_CREAT);
    if(shmid<0) {
        perror("Calling shmget");
        exit(1);
    }

    // attach shared memory
    dm = (struct dimensions*)shmat(shmid, 0, 0);
    if(dm==(void *)-1) {
        perror("Attaching shared memory buffer");
        exit(1);
    }

    // modify the data max value
    dm->data_max_val = data_max_val;

    return dm;
}

void get_data(struct dimensions *dm, int data_cur_val) {
    // set start to 1 to signal progress bar to start
    dm->start = 1;
    // modify the data in the shared memory
    dm->data_cur_val = data_cur_val;

}

void finish(struct dimensions *dm) {
    // detach shared memory
    shmdt(dm);
}
