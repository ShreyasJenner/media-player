#ifndef INIT_H
#define INIT_H

#include "stdheader.h"

#define PANEL_COUNT 4

void tui_init();

WINDOW **create_windows();

void box_windows(WINDOW **wins);

PANEL **create_panels(WINDOW **wins);

void destroy_windows(WINDOW **wins);

void destroy_panels(PANEL **panels);

void tui_update(WINDOW **wins);

void tui_deinit();

#endif // !INIT_H
