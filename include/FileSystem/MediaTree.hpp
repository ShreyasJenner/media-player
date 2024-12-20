#ifndef MEDIA_TREE_HPP
#define MEDIA_TREE_HPP

#include "FileSystem/FileSystemEnums.hpp"
#include <iostream>
#include <queue>
#include <string>

class Node {
public:
  std::wstring path;
  std::wstring filename;
  // bool that is set to true if pointing to a track
  bool track;

  NodeType type;
  Node *sibling = nullptr;
  Node *child = nullptr;
};

// Using First Child/Next Sibling representation
class MediaTree {
private:
  FileSystemMode mode;
  Node *root = nullptr;
  int size = 1;
  void helperDisplayMediaTree(Node *ptr, int tab);

public:
  MediaTree(FileSystemMode mode);

  void incrementSize();

  int getSize();

  std::vector<Node *> getAllPtrs();

  std::vector<Node *> getAlbumPtrs();

  std::vector<Node *> getArtistPtrs();

  std::vector<Node *> getTrackPtrs();

  Node *getRoot();

  void displayMediaTree();

  ~MediaTree();
};

#endif // !MEDIA_TREE_HPP
