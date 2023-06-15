# === Variables ===

# Important variables which the build process depends on
CC := gcc
BUILD_DIR := build

# Default compiler flags (none for now)
CFLAGS :=

# Default linker flags to link with Raylib
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

ifdef RAYGUI
	CFLAGS += -DRAYGUI_H_LOC=\"$(RAYGUI)\"
endif

ifeq ($(address), true)
	CFLAGS += -fsanitize=address -g
	LDFLAGS += -fsanitize=address -g
endif

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)
OUTFILE := space_test.elf

# Windows-specific variable changes (WORK IN PROGRESS, under testing)
ifeq ($(OS), Windows_NT)
	LDFLAGS := --static -lraylib -lopengl32 -lgdi32 -lwinmm
	OUTFILE := space_test.exe
endif

# === Targets ===

all: dirs compile link

$(OUTFILE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: dirs, compile, link, run, clean, help

# SRC_DIR is not made, expected to already exist
dirs:
	mkdir -p $(BUILD_DIR)

compile: $(OBJECTS)

link: $(OUTFILE)

run:
	./$(OUTFILE)

clean:
	rm -r $(BUILD_DIR)
	rm -f $(OUTFILE)

help:
	@echo "Demo space game Makefile: make [target] [options]"
	@echo "  - Available targets: dirs, compile, link, run, clean, help, all (default, does dirs, compile and link)"
	@echo "  - Available options:"
	@echo "    - address=true - turn on address sanitizer"
	@echo "    - RAYGUI=<path> - specify custom path to the raygui.h header"
	@echo "        (note: default expected raygui path is ../raygui/src/raygui.h)"
