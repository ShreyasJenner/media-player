all:
	#gcc -o media_player media_player.c progress-bar/progress_bar.c progress-bar/writer.c -lSDL2_mixer -lSDL2 -lpthread
	#gcc -o media_dets media_det.c -lSDL2_mixer -lSDL2
	@gcc -o bin/media-stream src/{stream.c,decode_mp3.c} -lmpg123 -lSDL2
