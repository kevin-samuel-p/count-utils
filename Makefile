CC ?= cc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    CFLAGS += -D_POSIX_C_SOURCE=200809L
endif

ifeq ($(UNAME_S),Darwin)
    CFLAGS += -D_POSIX_C_SOURCE=200809L
endif

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

.PHONY: all clean directories