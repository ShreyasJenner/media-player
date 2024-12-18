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

/* Function to print media tree with nice formatting */
void MediaTree::displayMediaTree() {
  Node *itr;
  int tab;

  // get root node of media tree and set tab
  itr = this->getRoot();
  tab = 0;

  // call helper function
  helperDisplayMediaTree(itr, tab);
}

/*
 * Helper function for displayMediaTree
 * Assumes first child - next sibling representation for tree
 */
void MediaTree::helperDisplayMediaTree(Node *ptr, int tab) {
  // print out twice tabs data
  std::cout << std::string(2 * tab, ' ');
  std::cout << ptr->data << '\n';

  // if is a track, then print all sibling tracks
  if (!ptr->track) {
    // set pointer to its child
    if (ptr->child != nullptr)
      helperDisplayMediaTree(ptr->child, tab + 1);

    // recurse siblings child
    while (ptr->sibling != nullptr && ptr->sibling->child != nullptr) {
      std::cout << std::string(2 * tab, ' ');
      std::cout << ptr->sibling->data << '\n';
      helperDisplayMediaTree(ptr->sibling->child, tab + 1);
      ptr = ptr->sibling;
    }
  } else {
    // print siblings
    while (ptr->sibling != nullptr) {
      std::cout << std::string(2 * tab, ' ');
      std::cout << ptr->sibling->data << '\n';
      ptr = ptr->sibling;
    }
  }
}

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
