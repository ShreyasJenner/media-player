#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "stdheader.h"
#include "structs.h"

MENU *start_screen_init(PANEL **panel, char filelist[][2][FILE_NAME_SZ],
                        int len);

const char *handle_keypress(MENU *menu, int key);

int start_screen_deinit(MENU *menu);

#endif // !START_SCREEN_H
