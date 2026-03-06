# ===== Project =====
TARGET := count

# ===== Detect Platform =====
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    PLATFORM := linux
else ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
else
    $(error Unsupported platform)
endif

# ===== Directories =====
OUTDIR := build/$(PLATFORM)
OBJDIR := $(OUTDIR)/obj

# ===== Compiler =====
CC ?= gcc
CFLAGS := -Wall -Wextra -std=c11 -Iinclude -D_POSIX_C_SOURCE=200809L

# ===== Sources =====
SRC := $(shell find src -name "*.c" ! -name "windows.c")

# ===== Objects =====
OBJ := $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))

# ===== Output Binary =====
TARGET_BIN := $(OUTDIR)/$(TARGET)

# ===== Build =====
all: directories $(TARGET_BIN)

$(TARGET_BIN): $(OBJ)
	$(CC) $(OBJ) -o $@

# ===== Compile =====
$(OBJDIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ===== Directories =====
directories:
	mkdir -p $(OBJDIR)

# ===== Clean =====
clean:
	rm -rf build

.PHONY: all clean directories