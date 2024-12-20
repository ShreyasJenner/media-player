#include "FileSystem/MediaTree.hpp"

/* Default constructor that initializes space for root node */
MediaTree::MediaTree(FileSystemMode mode) {
  this->root = new Node;
  this->mode = mode;
}

/* Function to increment size counter in media tree */
void MediaTree::incrementSize() { this->size++; }

/* Function to get size of media tree */
int MediaTree::getSize() { return this->size; }

/* Function to get vector of pointers to all nodes in the media tree */
std::vector<Node *> MediaTree::getAllPtrs() {
  std::vector<Node *> node_ptr_list;
  std::queue<Node *> node_ptr_queue;

  Node *itr, *ptr;

  // get root node pointer of media tree and push into queue
  itr = this->getRoot();
  node_ptr_queue.push(itr);

  // store all nodes into queue and store them into list
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

    // store node ptrs into vector
    node_ptr_list.push_back(itr);
  }

  return node_ptr_list;
}

/* Function to get node pointers to all nodes marked as album */
std::vector<Node *> MediaTree::getAlbumPtrs() {
  std::vector<Node *> album_ptr_list;
  std::queue<Node *> node_ptr_queue;

  Node *itr, *ptr;

  // get root node pointer of media tree and push into queue
  itr = this->getRoot();
  node_ptr_queue.push(itr);

  // store all nodes into queue and store them into list
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
    if (!(itr->type == NODE_ALBUM) && itr->child != nullptr) {
      node_ptr_queue.push(itr->child);
    }

    // store album node ptr into vector
    if (itr->type == NODE_ALBUM)
      album_ptr_list.push_back(itr);
  }

  return album_ptr_list;
}

/* Function to get string pointers to all nodes marked as artist */
std::vector<Node *> MediaTree::getArtistPtrs() {
  std::vector<Node *> artist_ptr_list;
  std::queue<Node *> node_ptr_queue;

  Node *itr, *ptr;

  // get root node pointer of media tree and push into queue
  itr = this->getRoot();
  node_ptr_queue.push(itr);

  // store all nodes into queue and push them into list
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
    if (!(itr->type == NODE_ARTIST) && itr->child != nullptr) {
      node_ptr_queue.push(itr->child);
    }

    // store artist node ptr into vector
    if (itr->type == NODE_ARTIST)
      artist_ptr_list.push_back(itr);
  }

  return artist_ptr_list;
}

/* Function to get node pointers to all nodes marked as tracks */
std::vector<Node *> MediaTree::getTrackPtrs() {
  std::vector<Node *> track_ptr_list;
  std::queue<Node *> node_ptr_queue;

  Node *itr, *ptr;

  // get root node pointer of media tree and push into queue
  itr = this->getRoot();
  node_ptr_queue.push(itr);

  // store all nodes into queue and push them into list
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
    if (!(itr->type == NODE_TRACK) && itr->child != nullptr) {
      node_ptr_queue.push(itr->child);
    }

    // store track node address into vector
    if (itr->type == NODE_TRACK)
      track_ptr_list.push_back(itr);
  }

  return track_ptr_list;
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
  // print out twice tabs filename
  std::cout << std::string(2 * tab, ' ');

  std::wcout << ptr->filename << '\n';

  // if is a track, then print all sibling tracks
  if (!ptr->track) {
    // set pointer to its child
    if (ptr->child != nullptr)
      helperDisplayMediaTree(ptr->child, tab + 1);

    // recurse siblings child
    while (ptr->sibling != nullptr && ptr->sibling->child != nullptr) {
      std::cout << std::string(2 * tab, ' ');
      std::wcout << ptr->sibling->filename << '\n';
      helperDisplayMediaTree(ptr->sibling->child, tab + 1);
      ptr = ptr->sibling;
    }
  } else {
    // print siblings
    while (ptr->sibling != nullptr) {
      std::cout << std::string(2 * tab, ' ');
      std::wcout << ptr->sibling->filename << '\n';
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
