TARGET := counter
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    PLATFORM := linux
else ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
else
    $(error Unsupported platform)
endif

CC ?= gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -D_POSIX_C_SOURCE=200809L

SRC = $(shell find src -name "*.c" ! -name "windows.c")
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

.PHONY: all clean directories