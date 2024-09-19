# Compiler and flags
CC=gcc
CFLAGS=-Wall -lncurses -lmenu -lpanel \
-lSDL2 -lSDL2_mixer \
-Iinclude -Iexternal \

# Directories
SRCDIR=src
OBJDIR=obj
LIBDIR=lib
PB_LIBDIR=external/progress-bar/lib
ID3_LIBDIR=external/id3reader/lib
BINDIR=bin

# Files
SRC=$(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/**/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
BIN=$(BINDIR)/media_player
LIB=$(LIBDIR)/media_player.a
PB_LIB=$(PB_LIBDIR)/progressbar.a 
ID3_LIB=$(ID3_LIBDIR)/id3reader.a 

# Libraries
LD_FLAGS=-L$(PB_LIBDIR) -L$(ID3_LIBDIR) \
		 -l:progressbar.a -l:id3reader.a


# Rules
all: $(BIN)

$(BIN): $(LIB) $(ID3_LIB) $(PB_LIB)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS) $(LD_FLAGS) 

$(LIB): $(OBJ) | $(LIBDIR)
	ar -rcs $@ $(OBJ) 

$(LIBDIR):
	mkdir -p $(LIBDIR)

$(PB_LIB): 
	cd external/progress-bar && make clean && make

$(ID3_LIB):
	cd external/id3reader && make clean && make

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

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
