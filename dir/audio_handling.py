from tinytag import TinyTag

# Class that handles grouping of audio files and implements the AudioHandling class in the dir module
class AudioHandling:
    def __init__(self) -> None:
        pass

    # Function that receives list of audio files and groups them based on artist
    # This includes the features as part of the artist string
    def group_by_artist(self, file_list: list) -> dict:
        artist_group = dict()
        for file in file_list:
            # Check if the file is supported in tinytag
            is_supported = TinyTag.is_supported(file)

            if is_supported:
                tag: TinyTag = TinyTag.get(file)
                # Check if artist is already listed
                if tag.artist not in artist_group:
                    artist_group[tag.artist] = []
                artist_group[tag.artist].append(file)

        return artist_group

    # Function that receives list of audio files and groups them based on album artist
    # This includes only the artist who created the album 
    def group_by_album_artist(self, file_list: list) -> dict: 
        album_artist_group = dict() 

        for file in file_list:
            # Check if the file is supported in tinytag
            is_supported = TinyTag.is_supported(file)

            if is_supported:
                tag: TinyTag = TinyTag.get(file)

                # Check if the albumartist tag does not exist
                if tag.albumartist is None:
                    tag.albumartist = tag.artist

                # If artist is not part of dict, add to dict
                if tag.albumartist not in album_artist_group:
                    album_artist_group[tag.albumartist] = []
                album_artist_group[tag.albumartist].append(file)

        return album_artist_group 


    # Function that receives list of audio files and groups them based on album 
    def group_by_album(self, file_list: list) -> dict:
        album_group = dict()
        for file in file_list:
            # Check if the file is supported in tinytag
            is_supported = TinyTag.is_supported(file)

            if is_supported:
                tag: TinyTag = TinyTag.get(file)
                # If album is not listed, create a new list
                if tag.album not in album_group:
                    album_group[tag.album] = []
                album_group[tag.album].append(file)

        return album_group

    # Function that does a complex grouping of the audio files
    # It first groups the list based on the album artist
    # It then does another grouping based on the album 
    # This gives a grouping similar to Artist -> [Album] -> [Tracks]
    def composite_grouping(self, file_list: list) -> dict:
        # Perform the initial group using album artist grouping
        album_artist_group = self.group_by_album_artist(file_list)

        composite_group = dict()

        # Go through the list of audio files for each album artist and then further group by album
        for album_artist in album_artist_group:
            audio_files = album_artist_group[album_artist]

            # Perform the album grouping for the grouped audio files
            album_group = self.group_by_album(audio_files)
            composite_group[album_artist] = album_group

        return composite_group
