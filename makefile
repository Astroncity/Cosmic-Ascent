# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O3 -L lib/ -I include/ -lraylib -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Output executable name
OUTPUT_NAME = main

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Colors for output

# Default target
all: directories $(BIN_DIR)/$(OUTPUT_NAME)
	$(info "Compilation complete. Executable '$(OUTPUT_NAME)' created in $(BIN_DIR).")

# Create directories if they don't exist
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Build the executable
$(BIN_DIR)/$(OUTPUT_NAME): $(OBJ_FILES)
	$(CC) $^ -o $@ $(CFLAGS)

# Compile each source file to an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(info "$(ACTION) Compiling $< to $@...")
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean the build and bin directories
clean:
	@echo "$(ACTION) Cleaning build and bin directories..."
	@rm -rf $(BUILD_DIR)/*
	@rm -rf $(BIN_DIR)/*

# Phony targets
.PHONY: all directories clean

