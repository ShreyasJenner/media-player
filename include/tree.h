#ifndef TREE_H
#define TREE_H

#include "stdheader.h"
#include "structs.h"

int get_ChildCount(char *dirname);

int store_FileDetails(char *dirname, struct Node *node, struct Tree *tree);

int get_DirTree(struct Tree *tree, char *dirname);

void free_Tree(struct Node *node);

void print_tree(struct Node *node, int count);

int search_Tree(struct Tree tree, char *pattern, char filelist[][FILE_NAME_SZ]);

struct Node *search_NodeWithValue(struct Tree tree, char *nodevalue);

#endif
