#ifndef TREE_H
#define TREE_H

#include "stdheader.h"
#include "structs.h"

int get_ChildCount(DIR *dir);

void store_FileDetails(char *dirname, struct Node *node, 
        struct Node *root);

void get_DirTree(struct Node *tree, char *dirname);

void free_Tree(struct Node *node);

void print_tree(struct Node *node);


#endif
