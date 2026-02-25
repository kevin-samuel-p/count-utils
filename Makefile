CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = $(shell find src -name "*.c")
OBJ = $(patsubst src/%.c,build/obj/%.o,$(SRC))

TARGET = build/counter

all: directories $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

build/obj/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p build/obj

clean:
	rm -rf build