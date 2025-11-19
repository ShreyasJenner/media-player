import os 

# Class that implements the FileHandling class in the dir module 
class FileHandling:
    # Class Variables
    path = []
    path_file = os.environ['HOME'] + "/.config/mp/dir.txt"


    # Default Constructor that performs the initial loading
    def __init__(self) -> None:
        self.load_paths()

    # Update directory path by adding a new path
    def update_path(self, path: str) -> None:
        self.path.append(path)

    # Delete a path from the directory
    def delete_path(self, idx: int) -> None:
        # Safety check on length of list
        if idx >= len(self.path):
            return
        self.path.pop(idx)

    # Get the list of available paths
    def get_paths(self) -> list:
        return self.path

    # Function that saves the list of paths to the disk
    def save_paths(self) -> None:
        with open(self.path_file, "w") as f:
            for paths in self.path:
                f.write(paths)
                f.write('\n')

    # Function that loads the list of paths from disk
    def load_paths(self) -> None:
        with open(self.path_file, "r") as f:
            for line in f:
                self.path.append(line.rstrip())

    # Function to get list of all files in a directory and its sub-directories 
    def get_files(self) -> list: 
        files = []
        # Get lists from all paths stored
        for paths in self.path:
            files.append([os.path.join(dirpath, f) for (dirpath, _, filenames) in os.walk(paths) for f in filenames])

        # Flatten the list
        return [x for xs in files for x in xs] 
