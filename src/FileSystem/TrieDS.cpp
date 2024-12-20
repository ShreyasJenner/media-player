#include "FileSystem/TrieDS.hpp"

/* Default Constructor for TrieNode class */
TrieNode::TrieNode() {
  int i;

  // set endofword to false
  this->endofword = false;

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
void Trie::insertWord(std::wstring *word, std::wstring suffix, NodeType type) {
  // if suffix is empty, then exit function
  if (suffix == L"") {
    return;
  }

  TrieNode *itr;
  int idx;

  // set iterator to root of trie
  itr = this->root;

  // iterate through characters of string
  for (char c : suffix) {

    // if character does not exist in hasmap, create a trienode and add it to
    // the hashmap
    if (itr->hashmap.find(c) == nullptr) {
      itr->hashmap[c] = new TrieNode();
    }

    // move iterator to child node
    itr = itr->hashmap[c];
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
 * Returns a set that stores all nodes that have strings that match the
 * corresponding pattern in their filename parameter
 * Set is used so that only unique word ptrs are returned
 */
std::set<std::tuple<std::wstring *, NodeType>>
Trie::searchWord(std::wstring word) {

  std::set<std::tuple<std::wstring *, NodeType>> node_set;
  std::queue<TrieNode *> list;
  TrieNode *itr;

  // set iterator to root node
  itr = this->root;

  // iterate through characters in the word
  for (char c : word) {

    // if index points to nullptr, word does not exist and exit loop
    if (itr->hashmap[c] == nullptr) {
      break;
    }
    itr = itr->hashmap[c];
  }

  // go through all of current nodes children and add to queue return all
  // possible words stored in those nodes
  list.push(itr);
  while (!list.empty()) {

    // pop the head of the list
    itr = list.front();
    list.pop();

    // search through the nodes hashmap and store all valid nodes
    for (std::pair<wchar_t, TrieNode *> child : itr->hashmap) {
      if (child.second != nullptr) {
        list.push(child.second);
        // if current node marks end of a word, then store all words and their
        // associated type into the node list
        if (child.second->endofword) {
          for (std::wstring *word : child.second->word_ptrs)
            node_set.insert(std::make_tuple(word, child.second->type));
        }
      }
    }
  }

  return node_set;
}

/* Function to search for artists
 * The function calls searchWord function and returns all artist pointers in the
 * vector obtained */
// TODO: check for better implementation of this function
// Possible alternatives are having 3 different tries; one for each possible
// value in NodeType
std::vector<std::wstring *> Trie::searchArtist(std::wstring word) {
  std::vector<std::wstring *> artist_list;
  std::set<std::tuple<std::wstring *, NodeType>> all_ptrs;

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
std::vector<std::wstring *> Trie::searchAlbum(std::wstring word) {
  std::vector<std::wstring *> album_list;
  std::set<std::tuple<std::wstring *, NodeType>> all_ptrs;

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
std::vector<std::wstring *> Trie::searchTrack(std::wstring word) {
  std::vector<std::wstring *> track_list;
  std::set<std::tuple<std::wstring *, NodeType>> all_ptrs;

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
bool Trie::startsWith(std::wstring prefix) {
  TrieNode *itr;

  // set iterator to root node
  itr = this->root;

  // iterate through characters in the word
  for (char c : prefix) {

    // if entry not in hashmap, word does not exist with given prefix
    if (itr->hashmap[c] == nullptr) {
      return false;
    }
    itr = itr->hashmap[c];
  }

  return true;
}

/* Function to display the trie */
void Trie::displayTrie(TrieNode *itr) {
  if (itr == nullptr)
    return;

  // iterate through iterators children
  for (std::pair<wchar_t, TrieNode *> pair : itr->hashmap) {
    if (pair.second != nullptr) {
      displayTrie(pair.second);
    }
  }

  if (itr->endofword == true) {
    for (std::wstring *word_ptr : itr->word_ptrs) {
      std::wcout << *word_ptr << '\n';
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
    for (std::pair<wchar_t, TrieNode *> pair : itr->hashmap) {
      // add the child to queue if it exists
      if (pair.second != nullptr) {
        elements.push(pair.second);
      }
    }

    // delete the node as it is no longer needed
    delete itr;
  }
}
