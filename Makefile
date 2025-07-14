# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iengine/include `sdl2-config --cflags`
LDFLAGS = `sdl2-config --cflags --libs`

# Directories and files
ENGINE_INC = engine/include
ENGINE_SRC = $(shell find engine/src -name "*.cpp")

BUILD_DIR = build/engine
LIB_DIR = /usr/local/lib
INCLUDE_DIR = /usr/local/include/engine

# Object files for engine
ENGINE_OBJS = $(patsubst engine/src/%.cpp, $(BUILD_DIR)/%.o, $(ENGINE_SRC))

# Static library name
LIBNAME = libengine.a
LIBPATH = $(BUILD_DIR)/$(LIBNAME)

# Default target: build library
all: $(LIBPATH)

# Build static library from objects
$(LIBPATH): $(ENGINE_OBJS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

# Compile source to object
$(BUILD_DIR)/%.o: engine/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Install library and headers to system
install: all
	@echo "Installing engine library and headers..."
	@sudo mkdir -p $(LIB_DIR) $(INCLUDE_DIR)
	@sudo cp $(LIBPATH) $(LIB_DIR)/
	@sudo cp -r engine/include/engine/* $(INCLUDE_DIR)/
	@echo "Installed to $(LIB_DIR) and $(INCLUDE_DIR)"

uninstall:
	@sudo rm -f $(LIB_DIR)/$(LIBNAME)
	@sudo rm -rf $(INCLUDE_DIR)
	@echo "Uninstalled from $(LIB_DIR) and $(INCLUDE_DIR)"

# Clean build files
clean:
	rm -rf build

.PHONY: all install clean


