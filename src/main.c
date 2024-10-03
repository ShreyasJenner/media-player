/* Main Program file */

/* header files */
// NOTE: global
#include "stdheader.h"
// NOTE:  local
#include "log.h"
#include "tui/init.h"

/* sub-module header files */
// NOTE:  global
#include "audio-metadata-reader/include/stdheader.h"
// NOTE:  local
#include "audio-metadata-reader/include/flac/flac_structs.h"
#include "audio-metadata-reader/include/flac/read_flac_metadata.h"

int main(int argc, char **argv) {
  /* check if file arg has been passed */
  if (argc < 2) {
    logerror(__FILE__, __LINE__, __func__, "File argument not passed");
    return 0;
  }

  /* Declaration */
  WINDOW **wins;
  PANEL **panels;

  /* initialize curses screen */
  tui_init();

  /* create windows and panels */
  wins = create_windows();
  panels = create_panels(wins);

  /* update tui to show screen */
  tui_update(wins);

  // NOTE: temp code to check if flac metadata is working
  FLACMetadata *metadata = get_FLACMetadata(argv[1]);
  if (metadata == NULL) {
    mvwprintw(wins[2], 1, 1, "%s\n", "Metadata failed baby");
  } else {
    mvwprintw(wins[2], 1, 1, "%s\n",
              metadata->vorbis_comment->data.vorbis_comment.comments[0].entry);
  }
  tui_update(wins);
  // NOTE: temp code to check if flac metadata is working

  // NOTE: block code to view result
  getch();

  /* clean up resources and log end of file */
  destroy_panels(panels);
  destroy_windows(wins);
  clean_FLACMetadata(metadata);
  tui_deinit();
  logend();

  return 0;
}
