/* Interface between start screen and media player program */

#include "interfaces/ss_mp.h"
#include "format.h"
#include "log.h"
#include "stdheader.h"
#include "structs.h"

/*
 * Function acts as an interface between the media player and the start screen
 * program. It takes the children of the node passed and stores it in a char
 * array It copies it into the char 3d array passed
 */
void interface_ss_mp(struct Node *node, char (*filepath)[FILE_NAME_SZ],
                     char (*filename)[FILE_NAME_SZ]) {
  /* Declaration */
  int i;

  /* Iterate through tree nodes and store in char array */
  i = 0;
  // NOTE: debugging statement
  while (i < node->childcount) {
    strcpy(filepath[i], node->children[i]->name);
    // sprintf(filepath[i], "%d", i);
    get_FileName(node->children[i]->name, filename[i]);
    // strcpy(filename[i], node->children[i]->name);
    i++;
  }
}
