/*
 * Functions defined in this file are used to get the list of all files and
 * directories stored in a directory passed as argument
 * Returns a tree data structure pointer, that stores the info in a tree format
 * Create a Node struct and then call the get_DirTree func
 * At the end of the program, call the free_Tree func
 */

#include "stdheader.h"
#include "structs.h"
#include <dirent.h>
#include <stdio.h>


/*
 * Get count of files and sub-dirs under dir passed
 * Exclude from the count '.' and '..'
 * Function does not close dir pointer or reset the stream position
 */
int get_ChildCount(DIR *dir) {
    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            count++;
        }
    }

    return count;
}


/*
 * Recursive Function to store file details in a node struct
 * Recurses if a node is a sub-dir and it has at least one child
 */
void store_FileDetails(char *dirname, struct Node *node, struct Node *root) {
    /* Declaration */
    DIR *dir, *childdir;
    struct dirent *entry;
    int i, tmp;

    /* Initialization */
    i = 0;

    /* directory opening error */
    dir = opendir(dirname);
    if(dir == NULL) {
        perror("opendir");
        return;
    }

    /* loop to iterate through dir entries */
    entry = readdir(dir);
    while(entry != NULL) {
        /* if the current node is not '.' or '..' */
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            node->children[i] = malloc(sizeof(struct Node));
            if (node->children[i] == NULL) {
                perror("malloc");
                closedir(dir);
                return;
            }

            /* prepend parent dir name to child dir */
            tmp = snprintf(node->children[i]->name, FILE_NAME_SZ, "%s/%s", dirname, entry->d_name);
            if(tmp>FILE_NAME_SZ) {
                perror("file name buffer too small");
                    closedir(dir);
                    return;
            }

            /* set children to NULL if node is a regular file */
            if (entry->d_type == DT_REG) {
                node->children[i]->type = 'f';
                node->children[i]->count = 0;
                node->children[i]->children = NULL;
            /* allocate space for children if node is a dir */
            } else if (entry->d_type == DT_DIR) {
                node->children[i]->type = 'd';
                /* get count of children nodes under this child node */
                childdir = opendir(node->children[i]->name);
                if(childdir == NULL) {
                    perror("opendir");
                    closedir(dir);
                    return;
                }
                node->children[i]->count = get_ChildCount(childdir);
                closedir(childdir);


                /* if children nodes exist, allocate space and recurse */
                if(node->children[i]->count > 0) {
                    node->children[i]->children = malloc(sizeof(struct Node *) * node->children[i]->count);
                    if (node->children[i]->children == NULL) {
                        perror("malloc");
                        closedir(dir);
                        return;
                    }
                    store_FileDetails(node->children[i]->name, node->children[i], root);
                /* else set children to NULL */
                } else {
                    node->children[i]->children = NULL;
                }
            }
            i++;
        }
        entry = readdir(dir);
    }
    closedir(dir);
}


/*

 * Function to create a tree containing the directory and file tree of the 
 * passed directory
 */
void get_DirTree(struct Node *tree, char *dirname) {
    /* Declaration */
    DIR *dir;

    /* Initialization */
    dir = opendir(dirname);

    if(dir == NULL) {
        perror("opendir");
        exit(1);
    }

    /* store dir details */
    strcpy(tree->name, dirname);
    tree->type = 'd';
    tree->count = get_ChildCount(dir);

    /* if the dir has sub-dirs/files, then allocate space for children nodes */
    if (tree->count > 0) {
        tree->children = malloc(sizeof(struct Node *) * tree->count);
        if (tree->children == NULL) {
            perror("malloc");
            closedir(dir);
            exit(EXIT_FAILURE);
        }
        store_FileDetails(dirname, tree, tree);
    } else {
        tree->children = NULL;
    }

    closedir(dir);
}


/*
 * Function to free the tree data structure created to store the file hierarchy
 */
void free_Tree(struct Node *node) {
    /* Declaration */
    int i;


    for (i = 0; i < node->count; i++) {
        /* if node is a dir, recurse on the node */
        if(node->children[i]->type == 'd') {
            free_Tree(node->children[i]);
        }
        free(node->children[i]);
    }

    /* free the children if not NULL */
    if(node->children!=NULL) {
        free(node->children);
    }
}

/*
 * Function to print tree
 */
void print_tree(struct Node *node) {
    int i;

    i = 0;
    while(i<node->count) {
        printf("%s\n",node->children[i]->name);
        if(node->children[i]->type=='d') {
            print_tree(node->children[i]);
        }
        i++;
    }
}
