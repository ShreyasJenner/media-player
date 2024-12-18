#ifndef TRIE_DS_HPP
#define TRIE_DS_HPP

#include <iostream>
#include <queue>
#include <string>
#include <vector>

#define CHILDREN_COUNT 96
#define UNICODE_OFFSET 32

class TrieNode {
public:
  bool endofword;

  // TODO: add support for script of japanese, korean, chinese and other
  // languages
  //
  // NOTE: Refer: https://en.wikipedia.org/wiki/List_of_Unicode_characters
  // The children array follow the format of unicode characters for Basic Latin
  // The array from 0 to 94 represent the characters as seen in the Basic Latin
  // section
  // UNICODE_OFFSET is subtracted from the integer value of the character to get
  // its index in the children array as the first 31 unicode characters are
  // special characters and don't need to be represented in the array
  TrieNode *children[96];

  std::vector<std::string> words;

  TrieNode();
};

class Trie {
private:
  TrieNode *root;

public:
  Trie();

  TrieNode *getRoot();

  void insertWord(std::string word, std::string suffix);

  std::vector<std::string> searchWord(std::string word);

  void deleteWord(std::string word);

  bool startsWith(std::string prefix);

  void displayTrie(TrieNode *itr);

  ~Trie();
};

#endif // !TRIE_DS_HPP
#define TRIE_DS_HPP
