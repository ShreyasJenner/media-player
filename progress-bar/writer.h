#ifndef WRITER_H
#define WRITER_H

#include "struct.h"

struct dimensions* init(int data_max_val);

void get_data(struct dimensions *dm, int data_cur_val);

void finish(struct dimensions *dm);

#endif
