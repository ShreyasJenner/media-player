#ifndef READDIR_HPP
#define READDIR_HPP

#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>

#include "FileSystem/MediaTree.hpp"
#include "FileSystem/TrieDS.hpp"

class FileSystem {
private:
  MediaTree *mt = NULL;
  Trie *trie = NULL;
  std::string path;

  void recursivePrint(std::string path, int tab);

public:
  FileSystem(std::string path);

  std::string getPath();

  Trie *getTrie();

  MediaTree *getMediaTree();

  void createMediaTree();

  void createTrie();

  void printMediaTree();

  ~FileSystem();
};

#endif // !READDIR_HPP
