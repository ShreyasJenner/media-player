all:
	gcc -o media_player media.c progress-bar/progress_bar.c progress-bar/writer.c -lSDL2_mixer -lSDL2 -lpthread
