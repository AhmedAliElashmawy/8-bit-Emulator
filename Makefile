# Compiler and linker settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LDFLAGS = -lSDL2

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj
ROM_DIR = roms

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE = $(BIN_DIR)/chip8

# Default target
all: directories $(EXECUTABLE)

# Create necessary directories
directories:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR) $(SRC_DIR) $(INC_DIR) $(ROM_DIR)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the emulator with a default ROM (change as needed)
run: all
	./$(EXECUTABLE) $(ROM_DIR)/Tetris\ [Fran\ Dachille,\ 1991].ch8

.PHONY: all directories clean run
