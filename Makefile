# ===== Project Name =====
TARGET := counter

# ===== Detect Platform =====
UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    PLATFORM := windows
else ifeq ($(UNAME_S),Linux)
    PLATFORM := linux
else ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
else
    $(error Unsupported platform)
endif

# ===== Compiler Selection =====
ifeq ($(PLATFORM),windows)
    CC := cl
    CFLAGS := /nologo /W4 /std:c11 /Iinclude
    OUTDIR := build/windows
    EXE := $(OUTDIR)/$(TARGET).exe
else
    CC := gcc
    CFLAGS := -Wall -Wextra -std=c11 -Iinclude -D_POSIX_C_SOURCE=200809L
    OUTDIR := build/$(PLATFORM)
    EXE := $(OUTDIR)/$(TARGET)
endif

# ===== Source Files =====
SRC_ALL := $(shell find src -name "*.c")

ifeq ($(PLATFORM),windows)
    SRC := $(filter-out %/posix.c,$(SRC_ALL))
else
    SRC := $(filter-out %/windows.c,$(SRC_ALL))
endif

OBJ := $(patsubst src/%.c,$(OUTDIR)/obj/%.o,$(SRC))

# ===== Build Targets =====
all: $(EXE)

$(EXE): $(OBJ)
ifeq ($(PLATFORM),windows)
	link /OUT:$@ $(OBJ)
else
	$(CC) $(OBJ) -o $@
endif

$(OUTDIR)/obj/%.o: src/%.c
	@mkdir -p $(dir $@)
ifeq ($(PLATFORM),windows)
	$(CC) $(CFLAGS) /c $< /Fo$@
else
	$(CC) $(CFLAGS) -c $< -o $@
endif

clean:
	rm -rf build

.PHONY: all clean