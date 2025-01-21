#ifndef READDIR_HPP
#define READDIR_HPP

#define MAX_PATH_LEN 255

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>

#include "FileSystem/FileSystemEnums.hpp"
#include "FileSystem/MediaTree.hpp"
#include "FileSystem/TrieDS.hpp"
#include "log/log.hpp"

class FileSystem {
private:
  MediaTree *mt;
  Trie *trie;
  std::wstring path;
  FileSystemMode mode;

public:
  FileSystem(std::wstring path, FileSystemMode mode);

  std::wstring getPath();

  Trie *getTrie();

  MediaTree *getMediaTree();

  void createMediaTree();

  void createTrie();

  ~FileSystem();
};

#endif // !READDIR_HPP
