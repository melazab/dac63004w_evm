# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -I./include -I/usr/include/libftdi1 -I/usr/local/include
LDFLAGS := -lftdi1 -lusb-1.0 -lft4222 -Wl,-rpath,/usr/local/lib

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Source subdirectories 
SRC_SUBDIRS := hardware operations utils waveforms
OBJS_SUBDIRS := $(addprefix $(OBJ_DIR)/,$(SRC_SUBDIRS)) 

# Find all source files
SRCS := $(wildcard $(SRC_DIR)/*.c) \
        $(foreach dir,$(SRC_SUBDIRS),$(wildcard $(SRC_DIR)/$(dir)/*.c))

# Generate object file paths
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Main executable name
TARGET := $(BIN_DIR)/dac63004

# Make all
.PHONY: all clean

all: dirs $(TARGET)

# Create necessary directories
dirs:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR) $(OBJ_SUBDIRS)

# Link the final executable
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Add dependency generation
DEPS := $(OBJS:.o=.d)
CFLAGS += -MMD -MP
-include $(DEPS)

# Debug target
.PHONY: debug
debug:
	@echo "Source files:"
	@echo "$(SRCS)"
	@echo
	@echo "Object files:"
	@echo "$(OBJS)"
