#include "FileSystem/FileSystem.hpp"
namespace fs = std::filesystem;

/* Parameterized constructor that stores the path and creates the MediaTree */
FileSystem::FileSystem(std::string path) {
  this->mt = new MediaTree();
  this->trie = new Trie();
  this->path = path;
}

/* Function to get Path */
std::string FileSystem::getPath() { return this->path; }

/* Function to get Trie */
Trie *FileSystem::getTrie() { return this->trie; }

/* Function to get Media Tree */
MediaTree *FileSystem::getMediaTree() { return this->mt; }

/* Function to create media tree */
void FileSystem::createMediaTree() {
  std::queue<Node *> entries;
  Node *itr;

  // set MediaTree root details
  itr = this->getMediaTree()->getRoot();
  itr->data = this->getPath();
  itr->track = false;
  itr->sibling = nullptr;
  itr->child = nullptr;

  // push root node into queue
  entries.push(itr);

  // while the queue is not empty
  while (!entries.empty()) {
    // set iterator to point to first element in queue and remove it
    itr = entries.front();
    entries.pop();

    Node *lastchild = nullptr;

    // iterate through the path and add it to the queue
    for (const auto &entry : fs::directory_iterator(itr->data)) {
      // handle symlinks that might cause infinite cyclic loops
      if (entry.is_symlink()) {
        std::cout << "Skipping symlink: " << entry.path() << '\n';
        continue;
      }

      // create a new node
      Node *ptr = new Node;
      ptr->data = entry.path();
      ptr->child = nullptr;
      ptr->sibling = nullptr;
      ptr->track = !entry.is_directory();

      // increase size count
      this->getMediaTree()->incrementSize();

      // if first child for node has not been set
      if (itr->child == nullptr) {
        // set iterator's child to first child pointer and set child to ptr
        itr->child = ptr;
      } else {
        // set the sibling for the child to the current pointer and update
        // {child}
        lastchild->sibling = ptr;
      }

      lastchild = ptr;

      // a directory cannot be a track and should be stored into queue
      if (entry.is_directory()) {
        entries.push(ptr);
      }
    }
  }
}

/*
 * Function to create trie
 * method: 1 = use insertWord(std::string *, std::string)
 * method: otherwise = use insertWord(std::string, std::string)
 * Method 1 saves on space and is used by default
 */
void FileSystem::createTrie(int method) {
  // store pointers to strings in the trie
  if (method == 1) {
    // set the method private variable in trie
    this->getTrie()->setMethod(method);

    // get a vector of pointers to string in the media tree
    MediaTree *mt = this->getMediaTree();
    std::vector<std::string *> word_list_ptr = mt->getStringAddrs();

    // iterate through vector and construct trie
    for (std::string *word_ptr : word_list_ptr) {
      this->getTrie()->insertWord(word_ptr, *word_ptr);
    }

  } else {
    this->getTrie()->setMethod(method);

    std::queue<std::string> entries;
    std::string itr;

    // push root node into queue
    entries.push(this->getPath());

    // while the queue is not empty
    while (!entries.empty()) {
      // set iterator to point to first element in queue and remove it
      itr = entries.front();
      entries.pop();

      // iterate through the path and add it to the queue
      for (const auto &entry : fs::directory_iterator(itr)) {

        // add the current entry to the trie
        this->getTrie()->insertWord(entry.path(), entry.path());

        // store only directory entries in queue
        if (entry.is_directory()) {
          entries.push(entry.path());
        }
      }
    }
  }
}

/* Function to print the MediaTree in a format similar to the output from the
 * linux command `tree` */
void FileSystem::printMediaTree() {
  if (this->getMediaTree() == nullptr) {
    std::cout << "Media Tree has not been created yet\n";
    return;
  }

  std::string path;
  int tab;

  // set itr to MediaTree root, tab to 0 and create the tuple element
  path = this->getMediaTree()->getRoot()->data;
  tab = 0;

  this->recursivePrint(path, tab);
}

/* Recursive helper function for `printMediaTree` to Recursively print the
 * MediaTree with nice formatting */
void FileSystem::recursivePrint(std::string path, int tab) {
  // print the node details with `tab` spaces
  std::cout << std::string(2 * tab, ' ');
  std::cout << path << '\n';

  // iterate through directory entries
  if (fs::is_directory(path)) {

    // iterate through the directory entries and recurse
    for (const auto &entry : fs::directory_iterator(path)) {
      if (fs::is_directory(entry.path())) {

        // create a new node pointer and recurse
        this->recursivePrint(entry.path(), tab + 1);
      } else {
        // print if not directory
        std::cout << std::string(2 * (tab + 1), ' ');
        std::cout << entry.path() << '\n';
      }
    }
  }
}

/* Destructor to get rid of all allocated space using new */
FileSystem::~FileSystem() {
  // Destructor of MediaTree is called when deleting the object
  delete this->getTrie();
  delete this->getMediaTree();
}
