#include "FileSystem/TrieDS.hpp"

/* Default Constructor for TrieNode class */
TrieNode::TrieNode() {
  int i;

  // set endofword to false and initialize all children pointers to null
  this->endofword = false;
  for (i = 0; i < CHILDREN_COUNT; i++) {
    this->children[i] = nullptr;
  }

  // set node type to NONE
  this->type = NODE_NONE;
}

/* Default Constructor to initialize TrieNode */
Trie::Trie() { this->root = new TrieNode(); }

/* Function to return pointer to root node */
TrieNode *Trie::getRoot() { return this->root; }

/*
 * Function to insert a word into the Trie
 * This function stores pointers to words in the media tree to save on space
 * It must be called after the creation of the media tree
 */
void Trie::insertWord(std::string *word, std::string suffix, NodeType type) {
  // if suffix is empty, then exit function
  if (suffix == "") {
    return;
  }

  TrieNode *itr;
  int idx;

  // set iterator to root of trie
  itr = this->root;

  // iterate through characters of string
  for (idx = 0; idx < suffix.length(); idx++) {
    int char_idx = suffix[idx] - UNICODE_OFFSET;

    // NOTE: korean characters occasionally take on a value less that that of
    // UNICODE_OFFSET.
    // The first if condition has been temporarily added to handle that
    //  TODO: add support for unicode characters of other langueges such as
    //  japanese, korean, chinese etc
    //  Currently does not support non-english characters
    if (char_idx < 0 || char_idx >= 96) {
      return;
    }

    // TODO: convert alphabets to lower case; handle special symbols such as $,
    // @ etc...
    //  if character index points to nullptr, create a new TrieNode
    if (itr->children[char_idx] == nullptr) {
      itr->children[char_idx] = new TrieNode();
    }

    // move iterator to child node
    itr = itr->children[char_idx];
  }

  // if word end has been reached, set endofword to true, store the word ptr in
  // the corresponding node and mark TrieNodeType

  itr->endofword = true;
  itr->type = type;
  itr->word_ptrs.push_back(word);

  // call the function recursively on the next suffix of the word
  this->insertWord(word, suffix.substr(1, suffix.length() - 1), type);
}

/*
 * Function to search for a word in the Trie
 * Returns a vector that stores all nodes that have strings that match the
 * corresponding pattern in their filename parameter
 */
std::vector<std::tuple<std::string *, NodeType>>
Trie::searchWord(std::string word) {

  std::vector<std::tuple<std::string *, NodeType>> node_list;
  std::queue<TrieNode *> list;
  TrieNode *itr;

  // set iterator to root node
  itr = this->root;

  // iterate through characters in the word
  for (char c : word) {
    int idx = c - UNICODE_OFFSET;

    // if index points to nullptr, word does not exist and exit loop
    if (itr->children[idx] == nullptr) {
      break;
    }
    itr = itr->children[idx];
  }

  // go through all of current nodes children and add to queue return all
  // possible words stored in those nodes
  list.push(itr);
  while (!list.empty()) {

    // pop the head of the list
    itr = list.front();
    list.pop();

    // search through the nodes children and store all valid nodes
    for (auto child : itr->children) {
      if (child != nullptr) {
        list.push(child);
        // if current node marks end of a word, then store all words and their
        // associated type into the node list
        if (child->endofword) {
          for (std::string *word : child->word_ptrs)
            node_list.push_back(std::make_tuple(word, child->type));
        }
      }
    }
  }

  return node_list;
}

/* Function to search for artists
 * The function calls searchWord function and returns all artist pointers in the
 * vector obtained */
// TODO: check for better implementation of this function
// Possible alternatives are having 3 different tries; one for each possible
// value in NodeType
std::vector<std::string *> Trie::searchArtist(std::string word) {
  std::vector<std::string *> artist_list;
  std::vector<std::tuple<std::string *, NodeType>> all_ptrs;

  // get pointers to nodes that match the given pattern
  all_ptrs = this->searchWord(word);

  // iterate through vector and get all artist nodes
  for (auto tuple : all_ptrs) {
    if (std::get<1>(tuple) == NODE_ARTIST) {
      artist_list.push_back(std::get<0>(tuple));
    }
  }

  return artist_list;
}

/* Function to search for albums
 * The function calls searchWord function and returns all album pointers in the
 * vector obtained */
// TODO: check for better implementation of this function
// Possible alternatives are having 3 different tries; one for each possible
// value in NodeType
std::vector<std::string *> Trie::searchAlbum(std::string word) {
  std::vector<std::string *> album_list;
  std::vector<std::tuple<std::string *, NodeType>> all_ptrs;

  // get pointers to nodes that match the given pattern
  all_ptrs = this->searchWord(word);

  // iterate through vector and get all artist nodes
  for (auto tuple : all_ptrs) {
    if (std::get<1>(tuple) == NODE_ALBUM) {
      album_list.push_back(std::get<0>(tuple));
    }
  }

  return album_list;
}

/* Function to search for tracks
 * The function calls searchWord function and returns all track pointers in the
 * vector obtained */
// TODO: check for better implementation of this function
// Possible alternatives are having 3 different tries; one for each possible
// value in NodeType
std::vector<std::string *> Trie::searchTrack(std::string word) {
  std::vector<std::string *> track_list;
  std::vector<std::tuple<std::string *, NodeType>> all_ptrs;

  // get pointers to nodes that match the given pattern
  all_ptrs = this->searchWord(word);

  // iterate through vector and get all artist nodes
  for (auto tuple : all_ptrs) {
    if (std::get<1>(tuple) == NODE_TRACK) {
      track_list.push_back(std::get<0>(tuple));
    }
  }

  return track_list;
}

/* Function to check if any word with given prefix exists in the Trie */
bool Trie::startsWith(std::string prefix) {
  TrieNode *itr;

  // set iterator to root node
  itr = this->root;

  // iterate through characters in the word
  for (char c : prefix) {
    int idx = c - UNICODE_OFFSET;

    // if index points to nullptr, word does not exist with given prefix
    if (itr->children[idx] == nullptr) {
      return false;
    }
    itr = itr->children[idx];
  }

  return true;
}

/* Function to display the trie */
void Trie::displayTrie(TrieNode *itr) {
  if (itr == nullptr)
    return;

  // iterate through iterators children
  for (TrieNode *child : itr->children) {
    if (child != nullptr) {
      displayTrie(child);
    }
  }

  if (itr->endofword == true) {
    for (std::string *word_ptr : itr->word_ptrs) {
      std::cout << *word_ptr << '\n';
    }
  }
}

/* Destructor to clean up the trie */
Trie::~Trie() {
  std::queue<TrieNode *> elements;
  TrieNode *itr;

  // set iterator to point to root node of Trie
  itr = this->getRoot();

  // iterate through the children of the TrieNode that itr is pointing to
  // currently and enqueue into the queue
  // Dequeue nodes from the queue and after enqueuing all existing children
  // delete the node
  elements.push(itr);

  // Keep the loop running until the queue is finally empty
  while (!elements.empty()) {
    // dequeue the element from the queue
    itr = elements.front();
    elements.pop();

    // iterate through the children of the current node
    for (TrieNode *child : itr->children) {
      // add the child to queue if it exists
      if (child != nullptr) {
        elements.push(child);
      }
    }

    // delete the node as it is no longer needed
    delete itr;
  }
}
