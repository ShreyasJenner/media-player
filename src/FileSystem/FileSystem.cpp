#include "FileSystem/FileSystem.hpp"
namespace fs = std::filesystem;

/* Parameterized constructor that stores the path and creates the MediaTree */
FileSystem::FileSystem(std::string path, FileSystemMode mode) {
  this->mt = new MediaTree(mode);
  this->trie = new Trie();
  this->path = path;
  this->mode = mode;
}

/* Function to get Path */
std::string FileSystem::getPath() { return this->path; }

/* Function to get Trie */
Trie *FileSystem::getTrie() { return this->trie; }

/* Function to get Media Tree */
MediaTree *FileSystem::getMediaTree() { return this->mt; }

/* Function to create media tree */
void FileSystem::createMediaTree() {
  // if mode being used is formatted dir structure
  if (this->mode == FORMATTED_DIR_STRUCTURE) {
    std::queue<Node *> entries;
    Node *itr;

    // set MediaTree root details
    itr = this->getMediaTree()->getRoot();
    itr->filename = this->getPath();
    itr->path = this->getPath();
    itr->track = false;
    itr->sibling = nullptr;
    itr->child = nullptr;
    itr->type = NODE_NONE;

    // push root node into queue
    entries.push(itr);

    // while the queue is not empty
    while (!entries.empty()) {
      // set iterator to point to first element in queue and remove it
      itr = entries.front();
      entries.pop();

      Node *lastchild = nullptr;

      // iterate through the path and add it to the queue
      for (const auto &entry : fs::directory_iterator(itr->path)) {
        // handle symlinks that might cause infinite cyclic loops
        if (entry.is_symlink()) {
          std::cout << "Skipping symlink: " << entry.path() << '\n';
          continue;
        } else if (entry.path().filename().string()[0] == '.') {
          // NOTE: use below line for debugging
          //  std::cout << "Skipping " << entry.path().filename().string() <<
          //  '\n';
          continue;
        }

        // create a new node and set the data based on itr
        Node *ptr = new Node;
        ptr->filename = entry.path().filename();
        ptr->path = entry.path();
        ptr->child = nullptr;
        ptr->sibling = nullptr;
        ptr->track = !entry.is_directory();
        ptr->type = (itr->type == NODE_NONE)     ? NODE_ARTIST
                    : (itr->type == NODE_ARTIST) ? NODE_ALBUM
                                                 : NODE_TRACK;

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
  } else {
    std::cout << "Mode is not yet supported\n";
  }
}

/* Function to create trie */
void FileSystem::createTrie() {

  // create trie based on mode
  if (this->mode == FORMATTED_DIR_STRUCTURE) {
    // get a vector of pointers to string in the media tree
    MediaTree *mt = this->getMediaTree();
    std::vector<Node *> word_list_ptr = mt->getAllPtrs();

    // iterate through vector and construct trie
    for (Node *node : word_list_ptr) {
      // store pointers to strings in the trie
      this->getTrie()->insertWord(&node->filename, node->filename, node->type);
    }
  } else {
    std::cout << "Mode not yet supported\n";
  }
}

/* Destructor to get rid of all allocated space using new */
FileSystem::~FileSystem() {
  // Destructor of MediaTree is called when deleting the object
  delete this->getTrie();
  delete this->getMediaTree();
}
