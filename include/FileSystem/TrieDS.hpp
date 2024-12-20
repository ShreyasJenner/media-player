#ifndef TRIE_DS_HPP
#define TRIE_DS_HPP

#include "FileSystem/FileSystemEnums.hpp"
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class TrieNode {
public:
  bool endofword;

  std::unordered_map<wchar_t, TrieNode *> hashmap;

  std::vector<std::wstring *> word_ptrs;

  // NOTE: initialized to NODE_NONE in the constructor
  NodeType type;

  TrieNode();
};

class Trie {
private:
  TrieNode *root;

public:
  Trie();

  TrieNode *getRoot();

  void insertWord(std::wstring *word, std::wstring suffix, NodeType type);

  std::set<std::tuple<std::wstring *, NodeType>> searchWord(std::wstring word);

  std::vector<std::wstring *> searchArtist(std::wstring word);

  std::vector<std::wstring *> searchAlbum(std::wstring word);

  std::vector<std::wstring *> searchTrack(std::wstring word);

  void deleteWord(std::wstring word);

  bool startsWith(std::wstring prefix);

  void displayTrie(TrieNode *itr);

  ~Trie();
};

#endif // !TRIE_DS_HPP
#define TRIE_DS_HPP
