#ifndef MEDIA_TREE_HPP
#define MEDIA_TREE_HPP

#include <queue>
#include <string>

class Node {
public:
  std::string data = "";
  // bool that is set to true if pointing to a track
  bool track;

  Node *sibling = NULL;
  Node *child = NULL;
};

// Using First Child/Next Sibling representation
class MediaTree {
private:
  Node *root = NULL;

public:
  MediaTree();

  Node *getRoot();

  ~MediaTree();
};

#endif // !MEDIA_TREE_HPP
