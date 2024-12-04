# compiler and flags
CXX=g++
CXXFLAGS=-Wall -Wextra -Iinclude -Iextern
DEMOFLAGS=-Iinclude -Iextern/audio-metadata-reader/include
LDFLAGS=-lSDL2 -lSDL2_mixer \
				-lftxui-component -lftxui-dom -lftxui-screen \
				-Lextern/FTXUI/_deps/ftxui-build \
				-lFLAC
STATICLD=extern/audio-metadata-reader/lib/metadata-reader.a

# Project Structure
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
LOG_DIR=$(HOME)/.local/state/media-player/
TARGET=$(BIN_DIR)/mediaplayer
DEMOTARGET=$(BIN_DIR)/demo

# find all .cpp files in SRC_DIR recursively and compile 
# corresponding object files in OBJ_DIR
SRCS=$(shell find $(SRC_DIR) -name '*.cpp')
OBJS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))


# build target
build: $(LOG_BUILD) $(TARGET)

# rule to create log directory
$(LOG_BUILD) : $(LOG_DIR)
	@mkdir -p $(LOG_DIR)
	@echo "Directory '$(LOG_DIR)' created"

# rule to build final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(STATICLD) $(LDFLAGS)


# rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# demo target
demo: $(LOG_DIR) $(DEMOTARGET)

# rule to create log directory
$(LOG_DIR):
	@mkdir -p $(LOG_DIR)
	@echo "Directory '$(LOG_DIR)' created"

# rule to build final executable
$(DEMOTARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(DEMOFLAGS) -o $@ $^ $(STATICLD) $(LDFLAGS)


# rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(DEMOFLAGS) -c $< -o $@


# clean build objects
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target
.PHONY: demo clean

