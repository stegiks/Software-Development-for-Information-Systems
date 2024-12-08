# Declare path variables
SRC := ./src
INCLUDE := ./include
INCLUDE_GTEST := ./googletest/googletest/include
BUILD := ./build
BIN := ./bin
DATASETS := ./datasets
TEST := ./tests

# Compilers
CC := gcc
CXX := g++

# Flags
CFLAGS := -Wall -Wextra -Werror -g -std=c++17 -lstdc++fs  -O3 -march=native -flto # After -std=c++17 optimization flags

# Path to local google test libraries
LDFLAGS := ./googletest/build/lib/libgtest.a ./googletest/build/lib/libgtest_main.a -pthread

# Arguments
ARGS := -b ./data/siftsmall_base.fvecs -q ./data/siftsmall_query.fvecs -f fvecs -gt ./data/siftsmall_groundtruth.ivecs -a 1 -R 50 -L 150
ARGS_FILT := -b ./data/dummy-data.bin -q ./data/dummy-queries.bin -f bin -a 1 -R 50 -L 150 -algo filter
ARGS_STITCH := -b ./data/dummy-data.bin -q ./data/dummy-queries.bin -f bin -a 1 -R 70 -L 150 -algo stitch

# Targets
SRC_FILES := $(wildcard $(SRC)/*.cpp)
OBJ_FILES := $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(SRC_FILES))

TEST_FILES := $(wildcard $(TEST)/*.cpp)
TEST_OBJ_FILES := $(patsubst $(TEST)/%.cpp, $(BUILD)/test_%.o, $(TEST_FILES))
TEST_ANN_OBJ_FILES := $(filter-out $(BUILD)/main.o, $(OBJ_FILES))

# Compilation
$(BUILD)/%.o: $(SRC)/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) -I$(INCLUDE)

# Linking
$(BIN)/main: $(OBJ_FILES)
	$(CXX) $^ -o $@ $(CFLAGS) -I$(INCLUDE)

# Test Compilation
$(BUILD)/test_%.o: $(TEST)/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) -I$(INCLUDE) -I$(INCLUDE_GTEST)

# Test executable
$(BIN)/tests: $(TEST_ANN_OBJ_FILES) $(TEST_OBJ_FILES)
	$(CXX) $^ -o $@ $(CFLAGS) $(LDFLAGS) -I$(INCLUDE) -I$(INCLUDE_GTEST)

# Run tests
test: $(BIN)/tests
	$(BIN)/tests

# Run
run: $(BIN)/main
	time $(BIN)/main $(ARGS)

run_filter: $(BIN)/main
	time $(BIN)/main $(ARGS_FILT)

run_stitch: $(BIN)/main
	time $(BIN)/main $(ARGS_STITCH)

# Clean
clean:
	rm -rf $(BUILD)/* $(BIN)/* *.txt *.out

# Valgrind
valgrind: $(BIN)/main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BIN)/main $(ARGS)

valgrind_filter: $(BIN)/main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BIN)/main $(ARGS_FILT)

valgrind_stitch: $(BIN)/main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BIN)/main $(ARGS_STITCH)

valgrind_tests: $(BIN)/tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BIN)/tests

all: clean test