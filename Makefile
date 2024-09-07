# Compiler and flags
CC = gcc
CFLAGS = -Wall -ncurses -SDL2 -SDL2_Mixer -Iinclude

# Directories
SRCDIR = src
OBJDIR = obj

# Files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

# Libraries
ID3_LIB = external/progress-bar/lib/id3reader.a


# Rules
all: $() $() $(OBJ)

pbar_make:
	cd external/progress-bar && make

id3_make:
	cd external/id3reader && make

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*.o

.PHONY: all clean
