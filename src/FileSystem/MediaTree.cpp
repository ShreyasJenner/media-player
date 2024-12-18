#include "FileSystem/MediaTree.hpp"

/* Default constructor that initializes space for root node */
MediaTree::MediaTree() { this->root = new Node; }

/* Function to increment size counter in media tree */
void MediaTree::incrementSize() { this->size++; }

/* Function to get size of media tree */
int MediaTree::getSize() { return this->size; }

/* Function to get vector of pointers to strings in the media tree */
std::vector<std::string *> MediaTree::getStringAddrs() {
  std::vector<std::string *> word_ptr_list;
  std::queue<Node *> node_ptr_queue;

  Node *itr, *ptr;

  // get root node pointer of media tree and push into queue
  itr = this->getRoot();
  node_ptr_queue.push(itr);

  // store all nodes into queue and get string data from them
  while (!node_ptr_queue.empty()) {
    // pop front of queue
    itr = node_ptr_queue.front();
    ptr = itr;
    node_ptr_queue.pop();

    // store valid siblings of node into queue
    if (ptr->sibling != nullptr) {
      node_ptr_queue.push(ptr->sibling);
      ptr = ptr->sibling;
    }

    // stores valid child of node into queue
    if (itr->child != nullptr) {
      node_ptr_queue.push(itr->child);
    }

    // store node string address into vector
    word_ptr_list.push_back(&itr->data);
  }

  return word_ptr_list;
}

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
