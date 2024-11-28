# compiler and flags
CXX=g++
CXXFLAGS=-Wall -Wextra -Iinclude
DEMOFLAGS=-Iinclude
LDFLAGS=-lSDL2 -lSDL2_mixer -lftxui-screen -lftxui-component \
				-lftxui-dom

# Project Structure
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
TARGET=$(BIN_DIR)/mediaplayer
DEMOTARGET=$(BIN_DIR)/demo

# find all .cpp files in SRC_DIR recursively and compile 
# corresponding object files in OBJ_DIR
SRCS=$(shell find $(SRC_DIR) -name '*.cpp')
OBJS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))


# build target
build: $(TARGET)

# rule to build final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


# rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# demo target
demo: $(DEMOTARGET)

# rule to build final executable
$(DEMOTARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(DEMOFLAGS) -o $@ $^ $(LDFLAGS)


# rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(DEMOFLAGS) -c $< -o $@


# clean build objects
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target
.PHONY: demo clean

