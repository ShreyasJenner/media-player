#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "stdheader.h"
#include "structs.h"

MENU *start_screen_init(PANEL **panel, char (*filepath)[FILE_NAME_SZ],
                        char (*filename)[FILE_NAME_SZ], int len);

const char *handle_keypress(MENU *menu, int key);

int start_screen_deinit(MENU *menu);

char *start_menu_run(PANEL **panel, struct Tree *tree);

#endif // !START_SCREEN_H
