
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -ggdb -L lib/ -I include/ -lraylib -lm
DEPFLAGS = -MMD -MP

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Output executable name
OUTPUT_NAME = cosmic-ascent

# Source files and object files
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRC_FILES:.c=.o))
DEP_FILES = $(OBJ_FILES:.o=.d)

# Colors for output
RED = \033[0;31m
GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[0;33m
RESET = \033[0m
ACTION = $(YELLOW) [ACTION] $(RESET)
INFO = $(BLUE) [INFO] $(RESET)

# Default target
all: directories $(BIN_DIR)/$(OUTPUT_NAME)
	@printf "$(INFO) Compilation complete. Executable '$(OUTPUT_NAME)' created in $(BIN_DIR).\n"

# Create directories if they don't exist
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Build the executable
$(BIN_DIR)/$(OUTPUT_NAME): $(OBJ_FILES)
	@printf "$(ACTION) Linking object files...\n"
	$(CC) $^ -o $@ $(CFLAGS)

# Compile each source file to an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "$(ACTION) Compiling $< to $@...\n"
	$(CC) -c $< -o $@ $(CFLAGS) $(DEPFLAGS)

# Include dependency files
-include $(DEP_FILES)

# Clean the build and bin directories
clean:
	@printf "$(ACTION) Cleaning build and bin directories...\n"
	@rm -rf $(BUILD_DIR)/*
	@rm -rf $(BIN_DIR)/*

# Phony targets
.PHONY: all directories clean
