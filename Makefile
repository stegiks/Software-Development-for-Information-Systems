# Declare path variables
SRC := ./src
INCLUDE := ./include
BUILD := ./build
BIN := ./bin
DATASETS := ./datasets

# Compilers
CC := gcc
CXX := g++

# Flags
CFLAGS := -Wall -Wextra -Werror -g # Maybe -std=c++17

# Arguments
ARGS := 

# Targets
SRC_FILES := $(wildcard $(SRC)/*.cpp)
OBJ_FILES := $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(SRC_FILES))

# Compilation
all: $(BIN)/main

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) -I$(INCLUDE)

# Linking
$(BIN)/main: $(OBJ_FILES)
	$(CXX) $^ -o $@ $(CFLAGS) -I$(INCLUDE)

# Run
run: $(BIN)/main
	$(BIN)/main $(ARGS)

# Clean
clean:
	rm -rf $(BUILD)/* $(BIN)/*

# Valgrind
valgrind: $(BIN)/main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BIN)/main $(ARGS)
