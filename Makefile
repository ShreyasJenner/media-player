all:
	@gcc -o bin/media_player src/{media_player.c,mp3_lut.c,key.c} \
	external/progress-bar/src/progress_bar.c \
	external/id3reader/src/{syncint.c,jpeg_writer.c,tag_reader.c,frame_reader.c,free.c} -lSDL2_mixer -lSDL2 -lpthread -lncurses
	@#gcc -o media_dets media_det.c -lSDL2_mixer -lSDL2
	@#gcc -o bin/media-stream src/{stream.c,decode_mp3.c} -lmpg123 -lSDL2
