# Compiler and flags
CC=gcc
CFLAGS=-Wall -g -lncurses -lmenu -lpanel \
-lSDL2 -lSDL2_mixer \
-lFLAC \
-Iinclude \

CSUBFLAGS=-Isub-modules \
					-Isub-modules/audio-metadata-reader/include \
					-Isub-modules/audio-metadata-reader/include/flac

# Directories
SRCDIR=src
OBJDIR=obj
BINDIR=bin
LIBDIR=lib
#PB_LIBDIR=external/progress-bar/lib
#ID3_LIBDIR=external/id3reader/lib
AUDIO_METADATA_LIBDIR=sub-modules/audio-metadata-reader/lib

# Files
MAIN=main
SRC=$(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/**/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
BIN=$(BINDIR)/media_player
LIB=$(LIBDIR)/media_player.a
#PB_LIB=$(PB_LIBDIR)/progressbar.a 
#ID3_LIB=$(ID3_LIBDIR)/id3reader.a 
AUDIO_METADATA_LIB=$(AUDIO_METADATA_LIBDIR)/metadata-reader.a

# Libraries
#LD_FLAGS=-L$(PB_LIBDIR) -L$(ID3_LIBDIR) \
#		 -l:progressbar.a -l:id3reader.a

LD_FLAGS=-L$(AUDIO_METADATA_LIBDIR) \
				 -l:metadata-reader.a


# Rules
all: $(BIN)

#$(BIN): $(LIB) $(ID3_LIB) $(PB_LIB)
$(BIN): $(LIB) $(AUDIO_METADATA_LIB)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS) $(CSUBFLAGS) $(LD_FLAGS) 

$(LIB): $(OBJ) | $(LIBDIR)
	ar -rcs $@ $(OBJ) 

$(LIBDIR):
	mkdir -p $(LIBDIR)

#$(PB_LIB): 
#	cd external/progress-bar && make clean && make
#
#$(ID3_LIB):
#	cd external/id3reader && make clean && make

$(AUDIO_METADATA_LIB):
	cd sub-modules/audio-metadata-reader && make clean && make

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CSUBFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(BIN)
	rm -f $(LIBDIR)/*.a
	rm -rf $(OBJDIR)/*

info:
	$(info SRC=$(SRC))
	$(info OBJ=$(OBJ))
	$(info EXT=$())

.PHONY: all clean info
