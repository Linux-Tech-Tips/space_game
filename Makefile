# === Variables ===

# Important variables which the build process depends on
CC := gcc
BUILD_DIR := build

# Default compiler flags (none for now)
CFLAGS :=

# Default linker flags to link with Raylib
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

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

.PHONY: dirs, compile, link, run, clean

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

