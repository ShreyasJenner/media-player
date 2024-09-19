/* Interface between start screen and media player program */

#include "interfaces/ss_mp.h"
#include "log.h"
#include "stdheader.h"
#include "structs.h"

/*
 * Function acts as an interface between the media player and the start screen
 * program. It takes the children of the node passed and stores it in a char
 * array It copies it into the char 3d array passed
 */
void interface_ss_mp(struct Node *node, char (*filelist)[2][FILE_NAME_SZ]) {
  /* Declaration */
  int i;

  /* Iterate through tree nodes and store in char array */
  i = 0;
  while (i < node->childcount) {
    sprintf(filelist[i][0], "%d", i);
    strcpy(filelist[i][1], node->children[i]->name);
    i++;
  }
}
