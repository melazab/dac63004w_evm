# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -I./include -I/usr/include/libftdi1
LDFLAGS := -lftdi1 -lusb-1.0

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Main executable name
TARGET := $(BIN_DIR)/dac_control

# Make all
.PHONY: all clean

all: dirs $(TARGET)

# Create necessary directories
dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link the final executable
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
