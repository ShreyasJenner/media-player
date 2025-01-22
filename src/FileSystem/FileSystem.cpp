#include "FileSystem/FileSystem.hpp"
namespace fs = std::filesystem;

/* Parameterized constructor that stores the path and creates the MediaTree */
FileSystem::FileSystem(std::wstring path, FileSystemMode mode) {
  this->mt = new MediaTree(mode);
  this->trie = new Trie();
  this->path = path;
  this->mode = mode;
  this->createMediaTree();
  this->createTrie();
}

/* Function to get Path */
std::wstring FileSystem::getPath() { return this->path; }

/* Function to get Trie */
Trie *FileSystem::getTrie() { return this->trie; }

/* Function to get Media Tree */
MediaTree *FileSystem::getMediaTree() { return this->mt; }

/* Function to create media tree */
void FileSystem::createMediaTree() {
  char temp_str[MAX_PATH_LEN * sizeof(wchar_t)];

  // if mode being used is formatted dir structure
  if (this->mode == FORMATTED_DIR_STRUCTURE) {
    std::queue<Node *> entries;
    Node *itr;

    // set MediaTree root details
    itr = this->getMediaTree()->getRoot();
    itr->wfilename = this->getPath();
    itr->wpath = this->getPath();
    // store the multi-byte encoded wide strings
    std::wcstombs(temp_str, itr->wpath.c_str(),
                  sizeof(wchar_t) * itr->wpath.length());
    itr->mbpath = temp_str;
    // store the multi-byte encoded wide strings
    std::wcstombs(temp_str, itr->wfilename.c_str(),
                  sizeof(wchar_t) * itr->wfilename.length());
    itr->mbfilename = temp_str;
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
      for (const auto &entry : fs::directory_iterator(itr->wpath)) {
        // handle symlinks that might cause infinite cyclic loops
        if (entry.is_symlink()) {
          std::cout << "Skipping symlink: " << entry.path() << '\n';
          continue;

          // skip directories that start with .
        } else if (entry.path().filename().string()[0] == '.') {
          logerror(__FILE__, __LINE__, __func__, LOGLEVEL::INFO,
                   "File with . ignored");
          continue;
        }

        // create a new node and set the data based on itr
        Node *ptr = new Node;
        ptr->wfilename = entry.path().stem().wstring();
        ptr->wpath = entry.path().wstring();
        // store the multi-byte encoded wide strings
        std::wcstombs(temp_str, ptr->wpath.c_str(),
                      sizeof(wchar_t) * ptr->wpath.length());
        ptr->mbpath = temp_str;
        // store the multi-byte encoded wide strings
        std::wcstombs(temp_str, ptr->wfilename.c_str(),
                      sizeof(wchar_t) * ptr->wfilename.length());
        ptr->mbfilename = temp_str;
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
    logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
             "Mode is not yet supported");
    return;
  }
}

/* Function to create trie */
void FileSystem::createTrie() {

  // create trie based on mode
  if (this->mode == FORMATTED_DIR_STRUCTURE) {
    // get a vector of pointers to string in the media tree
    MediaTree *mt = this->getMediaTree();
    if (mt == nullptr) {
      logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
               "MediaTree has not been created");
      return;
    }

    std::vector<Node *> word_list_ptr = mt->getAllPtrs();

    // iterate through vector and construct trie
    for (Node *node : word_list_ptr) {
      // store pointers to strings in the trie
      this->getTrie()->insertWord(&node->wfilename, node->wfilename,
                                  node->type);
    }
  } else {
    logerror(__FILE__, __LINE__, __func__, LOGLEVEL::ERROR,
             "Mode not yet supported");
    return;
  }
}

/* Destructor to get rid of all allocated space using new */
FileSystem::~FileSystem() {
  // Destructor of MediaTree is called when deleting the object
  delete this->getTrie();
  delete this->getMediaTree();
}
