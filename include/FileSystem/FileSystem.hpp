#ifndef READDIR_HPP
#define READDIR_HPP

#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>

#include "FileSystem/FileSystemEnums.hpp"
#include "FileSystem/MediaTree.hpp"
#include "FileSystem/TrieDS.hpp"

class FileSystem {
private:
  MediaTree *mt;
  Trie *trie;
  std::string path;
  FileSystemMode mode;

public:
  FileSystem(std::string path, FileSystemMode mode);

  std::string getPath();

  Trie *getTrie();

  MediaTree *getMediaTree();

  void createMediaTree();

  void createTrie();

  ~FileSystem();
};

#endif // !READDIR_HPP
