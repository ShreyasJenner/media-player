/* Header file to store all file system related enums */
#ifndef FILE_SYSTEM_ENUMS_H
#define FILE_SYSTEM_ENUMS_H

/* NOTE:
 * File system class operates in two ways
 * 0 = NAIVE_FILE_MANAGER: In naive file manager mode, file system reads the
 * directory under the path that is passed on intialization of the FileSystem
 * object. A first child - next siblings tree is built and is directly show in
 * the tui In this state, the program just acts as a specialized file manager
 * with support for playing music
 * 1 = FORMATTED_DIR_STRUCTURE: In formatted dir structure mode, file system
 * assumes a certain directory configuration.
 * The format is:
 * Root Dir
 * --Artist1
 * ----Album1
 * ------Track1
 * ------...
 * ------TrackN
 * ----Album2
 * ----...
 * ----AlbumN
 * --Artist2
 * --...
 * --ArtistN
 * In such a format, the file system will be faster, and a lot of functions
 * offered by the file system will be available and faster. Such a directory
 * configuration can be achieved either manually or through audio tagging
 * software, such as beet
 */
enum FileSystemMode { NAIVE_FILE_MANAGER, FORMATTED_DIR_STRUCTURE };

/*NOTE:
 * NodeType enum represents what subject the string data in the node is about
 * It is set in the media tree and is currently only possible when
 * FileSystemMode is set to FORMATTED_DIR_STRUCTURE
 */
enum NodeType { NODE_NONE, NODE_ARTIST, NODE_ALBUM, NODE_TRACK };

#endif // !FILE_SYSTEM_ENUMS_H
