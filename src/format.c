#include "stdheader.h"


/*
 * Function to get name of file from absolute path provided
 */
void get_FileName(char *pathname, char *filename) {
    /* Declaration */
    char *pos;


    pos = (strrchr(pathname, '/'));
    strcpy(filename, pos+1);
}
