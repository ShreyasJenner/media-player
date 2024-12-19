#ifndef READDIR_HPP
#define READDIR_HPP

#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>

#include "FileSystem/MediaTree.hpp"
#include "FileSystem/TrieDS.hpp"

// NOTE: File system class operates in two ways
// 0 = NAIVE_FILE_MANAGER: In naive file manager mode, file system reads the
// directory under the path that is passed on intialization of the FileSystem
// object. A first child - next siblings tree is built and is directly show in
// the tui In this state, the program just acts as a specialized file manager
// with support for playing music
//
// NOTE:
// 1 = FORMATTED_DIR_STRUCTURE: In formatted dir structure mode, file system
// assumes a certain directory configuration.
// The format is:
// Root Dir
// --Artist1
// ----Album1
// ------Track1
// ------...
// ------TrackN
// ----Album2
// ----...
// ----AlbumN
// --Artist2
// --...
// --ArtistN
// In such a format, the file system will be faster, and a lot of functions
// offered by the file system will be available and faster. Such a directory
// configuration can be achieved either manually or through audio tagging
// software, such as beet
enum FileSystemMode { NAIVE_FILE_MANAGER, FORMATTED_DIR_STRUCTURE };

class FileSystem {
private:
  MediaTree *mt = NULL;
  Trie *trie = NULL;
  std::string path;
  FileSystemMode mode;

public:
  FileSystem(std::string path);

  std::string getPath();

  Trie *getTrie();

  MediaTree *getMediaTree();

  void createMediaTree();

  // NOTE: method = 1 => use string pointers
  //  otherwise => use strings
  //  Read comments above function for further details
  void createTrie();

  ~FileSystem();
};

#endif // !READDIR_HPP
