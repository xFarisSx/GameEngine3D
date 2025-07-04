# Compiler and flags
CC = g++
CFLAGS = -std=c++17 -Wall -Iengine/include `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Directories
ENGINE_DIR = engine
GAME_DIR = game
# ========== Paths ==========
ENGINE_INC := engine/include
ENGINE_SRC := $(wildcard engine/src/**/*.cpp engine/src/*.cpp)

GAME_INC := game/include
GAME_SRC := $(wildcard game/src/*.cpp)

BUILD_DIR := build
BIN := ~/CppProjectBuild/main

# ========== Compiler and Flags ==========
CXX := g++
CXXFLAGS := -std=c++17 -I$(ENGINE_INC) -I$(GAME_INC) `sdl2-config --cflags` -Wall -Wextra
LDFLAGS := `sdl2-config --libs`

# ========== Object Files ==========
OBJS := $(ENGINE_SRC:.cpp=.o) $(GAME_SRC:.cpp=.o)
OBJS := $(patsubst %,$(BUILD_DIR)/%,$(OBJS))

# ========== Rules ==========

# Default target
all: $(BIN)

# Link final binary
$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $^ $(LDFLAGS) -o $@
	@echo "Built: $@"

# Compile .cpp → .o
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

