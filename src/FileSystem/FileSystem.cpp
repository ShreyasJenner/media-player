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

/* Function to create trie */
void FileSystem::createTrie(int method) {

  // get a vector of pointers to string in the media tree
  MediaTree *mt = this->getMediaTree();
  std::vector<std::string *> word_list_ptr = mt->getStringAddrs();

  // iterate through vector and construct trie
  for (std::string *word_ptr : word_list_ptr) {
    // store pointers to strings in the trie
    this->getTrie()->insertWord(word_ptr, *word_ptr);
  }
}

/* Destructor to get rid of all allocated space using new */
FileSystem::~FileSystem() {
  // Destructor of MediaTree is called when deleting the object
  delete this->getTrie();
  delete this->getMediaTree();
}
