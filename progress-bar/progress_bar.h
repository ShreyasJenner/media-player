#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

void refresh_dim(int signum);

int map_into_range(int val, int term_width, int data_max);

void* progress_bar(void *args);

#endif
