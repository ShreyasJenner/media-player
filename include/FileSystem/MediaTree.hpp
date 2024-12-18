#ifndef MEDIA_TREE_HPP
#define MEDIA_TREE_HPP

#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>

class Node {
public:
  std::string data = "";
  // bool that is set to true if pointing to a track
  bool track;

  Node *sibling = nullptr;
  Node *child = nullptr;
};

// Using First Child/Next Sibling representation
class MediaTree {
private:
  Node *root = nullptr;
  int size = 1;

public:
  MediaTree();

  void incrementSize();

  int getSize();

  std::vector<std::string *> getStringAddrs();

  Node *getRoot();

  void traverseTree();

  ~MediaTree();
};

#endif // !MEDIA_TREE_HPP
