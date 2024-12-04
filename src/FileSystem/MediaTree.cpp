#include "FileSystem/MediaTree.hpp"

/* Default constructor that initializes space for root node */
MediaTree::MediaTree() { this->root = new Node; }

/* Function to return pointer to media tree root node */
Node *MediaTree::getRoot() { return this->root; }

/* Destructor to get rid of all space allocated with new */
MediaTree::~MediaTree() {
  Node *itr, *child;
  std::queue<Node *> elements;

  // set `itr` to point to root node and push into queue
  itr = this->getRoot();
  elements.push(itr);

  // while all elements have not yet been deallocated
  while (!elements.empty()) {
    itr = elements.front();
    elements.pop();

    // if the `itr` node has a child
    if (itr->child != NULL) {

      // push the connected child into the queue
      elements.push(itr->child);

      // push the iterators childs siblings (if they exist) into the queue
      child = itr->child->sibling;
      while (child != NULL) {
        elements.push(child);
        child = child->sibling;
      }
    }

    // delete the iterator since there is no more need for it
    delete itr;
  }
}
