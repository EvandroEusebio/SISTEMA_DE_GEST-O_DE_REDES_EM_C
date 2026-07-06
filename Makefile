# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Directories
SRC_DIR = src
INC_DIR = include
OUT_DIR = output

# Output
TARGET = $(OUT_DIR)/main

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/grafo.c $(SRC_DIR)/dispositivo.c $(SRC_DIR)/login.c $(SRC_DIR)/persistencia.c
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Create output directory if needed
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Link
$(TARGET): $(OBJS) | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c to .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

# Clean
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
