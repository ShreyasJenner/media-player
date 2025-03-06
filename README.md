Attempt to implement media player

# To-DO:
- sort the file list returned by search tree in order of parent folder and track number

- Trie data structure is used for searching, implement Aho-Corasick algorithm



### Time Measurements (Average of 10 samples)
- createMediaTree : 3134 microseconds
- createTrie : 45671.4 microseconds

##### Commands used to get time taken by function
```C++
auto start = std::chrono::high_resolution_clock::now();

auto stop = std::chrono::high_resolution_clock::now();
  std::cout << "Time taken :"
            << std::chrono::duration_cast<std::chrono::microseconds>(stop -
                                                                     start)
                   .count()
            << '\n';
```
