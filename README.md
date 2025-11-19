Attempt to implement media player in python

# Features:
- support for synced lyrics
- plays files
- possibly playlist creation


# Roadmap:
## Directory Module
- Build module that gets list of audio files recursively
- When starting the program, the user can add a path to a directory and this is remembered by the program
- The user can update the library, and this will rescan the directory and add new entries to the list of files available
- Group the audio files using the metadata in them
## GUI module 
  - Build module using pyside6 that creates a gui for the program 
  - The module uses the directory module as a backend
  - The module can display different views of the audio files
    - View 1: Artist view (group by artist)
    - View 2: Album view (group by album)
    - View 3: File view (no grouping)
  - Search is implemented
  - Synced lyrics is supported when possible
