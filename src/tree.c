/*
 * Functions defined in this file are used to get the list of all files and
 * directories stored in a directory passed as argument
 * Returns a tree data structure pointer, that stores the info in a tree format
 * Create a Node struct and then call the get_DirTree func
 * At the end of the program, call the free_Tree func
 */

#include "stdheader.h"
#include "structs.h"
#include <SDL2/SDL_shape.h>

/*
 * Get count of files and sub-dirs under dir passed
 * Exclude from the count '.' and '..'
 * Function handles opening and closing of directory stream
 */
int get_ChildCount(char *dirname) {
    /* Declaration */
    DIR* dir;
    struct dirent *entry;
    int count;

    /* Initialization */
    dir = opendir(dirname);
    if(dir == NULL) {
        printf("Error in function %s:%s\n",__func__,"opening directory");
        return -1;
    }
    count = 0;

    /* 
     * iterate through dir stream and get count of all dirs/files under the 
     * passed directory 
     */
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            count++;
        }
    }

    /* close dir pointer */
    closedir(dir);

    return count;
}


/*
 * Recursive Function to store file details in a node struct that 
 * recurses if a node is a sub-dir and is not empty
 * Node names are stored as absolute paths
 * Eg: /media is main file; /media/example is an example directory/file 
 * under media
 */
void store_FileDetails(char *dirname, struct Node *node, struct Tree *tree) {
    /* Declaration */
    DIR *dir;
    struct dirent *entry;
    int i, tmp;

    /* Initialization */
    i = 0;

    /* open directory */
    dir = opendir(dirname);
    if(dir == NULL) {
        perror("opening dir for storing node details");
        return;
    }

    /* loop to iterate through dir entries */
    for(entry=readdir(dir);entry!=NULL;entry=readdir(dir)) {
        if(entry == NULL) {
            printf("Error in function %s: %s\n",__func__,"reading directory stream");
            exit(1);
        }
        /* if the current node is not '.' or '..' */
        if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            /* allocate space for node's child */
            node->children[i] = malloc(sizeof(struct Node));
            if(node->children[i] == NULL) {
                perror("malloc");
                closedir(dir);
                return;
            }

            /* prepend parent dir name to child dir */
            tmp = snprintf(node->children[i]->name, FILE_NAME_SZ, "%s/%s", dirname, entry->d_name);
            if(tmp>=FILE_NAME_SZ) {
                perror("file name buffer too small");
                    closedir(dir);
                    return;
            }

            /* set children to NULL if node is a regular file */
            if(entry->d_type == DT_REG) {
                /* update file count of tree */
                tree->filecount++;

                node->children[i]->type = 'f';
                node->children[i]->childcount = 0;
                node->children[i]->children = NULL;

            /* allocate space for children if node is a dir */
            } else if(entry->d_type == DT_DIR) {
                /* update directory count of tree */
                tree->dircount++;

                node->children[i]->type = 'd';
                /* 
                 * get count of children nodes under this child node and 
                 * allocate space if they exist
                 */
                node->children[i]->childcount = get_ChildCount(node->children[i]->name);
                if(node->children[i]->childcount > 0) {
                    node->children[i]->children = malloc(sizeof(struct Node *) * node->children[i]->childcount);
                    if(node->children[i]->children == NULL) {
                        perror("malloc");
                        closedir(dir);
                        return;
                    }
                    store_FileDetails(node->children[i]->name, node->children[i], tree);
                /* else set children to NULL */
                } else {
                    node->children[i]->children = NULL;
                }
            }
            /* increment i only if given entry is not '.' or '..' */
            i++;
        }
    }
    closedir(dir);
}


/*

 * Function to create a tree containing the directory and file tree of the 
 * passed directory
 */
void get_DirTree(struct Tree *tree, char *dirname) {

    /* store dir details */
    strcpy(tree->root.name, dirname);
    tree->root.type = 'd';
    tree->root.childcount = get_ChildCount(dirname);
    tree->dircount = 1;
    tree->filecount = 0;

    /* if the dir has sub-dirs/files, then allocate space for children nodes */
    if(tree->root.childcount > 0) {
        tree->root.children = malloc(sizeof(struct Node *) * tree->root.childcount);
        if (tree->root.children == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        /* store data for children nodes */
        store_FileDetails(dirname, &(tree->root), tree);
    } else {
    /* set children to NULL */
        tree->root.children = NULL;
    }
}


/*
 * Function to free the tree data structure created to store the file hierarchy
 */
void free_Tree(struct Node *node) {
    /* Declaration */
    int i;


    for (i = 0; i < node->childcount; i++) {
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
void print_tree(struct Node *node, int count) {
    int i,j;

    i = 0;
    while(i<node->childcount) {
        for(j=0;j<count;j++)
            printf("\t");

        printf("%s\n",node->children[i]->name);
        if(node->children[i]->type=='d') {
            print_tree(node->children[i],count+1);
        }
        i++;
    }
}

/*
 * Function does bfs on tree and uses regex to compare the passed string to the file names
 * Copies list of files that match the given pattern into pattern arg
 * Size of double array passed can be gotten from tree.filecount and FILE_NAME_SZ macro
 * Returns number of matching files found
 *
 * Arguments:
 * pattern: stores files that match the regex
 * Variables:
 * dirqueue: stores directories
 */
int search_Tree(struct Tree tree, char *pattern, char filelist[][FILE_NAME_SZ]) {
    /* Declaration */
    int i, j, filelist_itr, queueend, err; 

    regex_t reg;
    struct Node *dirqueue[tree.dircount];
    struct Node *itr;



    /* Initialization/Allocating Space */
    i = 0;
    filelist_itr = 0;
    dirqueue[i] = &(tree.root);
    queueend = 1;
    err = regcomp(&reg, pattern, REG_ICASE);
    if(err != 0) {
        printf("Reg comp error\n");
        exit(1);
    }

    /* while queue isnt empty */
    while(i!=queueend) {
        /* get first node of queue and then dequeue */
        itr = dirqueue[i];
        i++;


        /* iterate through the current nodes children */
        for(j=0;j<itr->childcount;j++) {
            /* add to dirqueue if a directory */
            if(itr->children[j]->type == 'd') {
                dirqueue[queueend++] = itr->children[j];
            } 
            /* copy to pattern array if pattern matches node name */
            else if(itr->children[j]->type == 'f') {
                if(regexec(&reg, itr->children[j]->name, 0, NULL, 0)==0) {
                    strcpy(filelist[filelist_itr++], itr->children[j]->name);
                }
            }
        }
    }
    regfree(&reg);

    /* return number of matches */
    return filelist_itr;
}
