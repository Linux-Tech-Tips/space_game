# === Variables ===

# Important variables which the build process depends on
CC := gcc
BUILD_DIR := build
SOURCE_DIR := src
OUTFILE := space_test.elf
DEPS_DIR := .dependencies

# Default compiler flags (none for now)
CFLAGS :=

# Default linker flags to link with Raylib
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Custom Raygui path
ifdef RAYGUI
	CFLAGS += -DRAYGUI_H_LOC=\"$(RAYGUI)\"
endif

# Custom Raylib path
ifdef RAYLIB_PATH
	CFLAGS += -L$(RAYLIB_PATH)/lib -I$(RAYLIB_PATH)/include
	LDFLAGS += -L$(RAYLIB_PATH)/lib -I$(RAYLIB_PATH)/include
endif

# Address sanitizer flags (if enabled)
ifeq ($(address), true)
	CFLAGS += -fsanitize=address -g
	LDFLAGS += -fsanitize=address -g
endif

# These variables are needed for the compile and link PHONY targets - which implicit rules to call
SOURCES := $(shell find $(SOURCE_DIR) -name *.c) # For any windows versions, the Cygwin find utility is required
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))

# Search path for the pattern rule
VPATH := $(shell find $(SOURCE_DIR) -type d)

# Windows-specific variable changes
ifeq ($(OS), Windows_NT)
	LDFLAGS := --static -lraylib -lopengl32 -lgdi32 -lwinmm
	OUTFILE := space_test.exe
endif


# === Targets ===

all: dirs compile link

$(OUTFILE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Pattern rule for compilation (+ generating dependency rules if not forbidden)
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
ifneq ($(NO_DEPS), true)
	$(CC) $(CFLAGS) -MT $@ -MM $< > $(DEPS_DIR)/$(notdir $@).dep
endif

.PHONY: dirs, compile, link, run, clean, help

# SOURCE_DIR is not made, expected to already exist
# If not specifically forbidden, creates the dependency targets directory as well
dirs:
	mkdir -p $(BUILD_DIR)
ifneq ($(NO_DEPS), true)
	mkdir -p $(DEPS_DIR)
endif

compile: $(OBJECTS)

link: $(OUTFILE)

run: all
	./$(OUTFILE)

# If not specifically forbidden, cleans everything including dependency rules
clean:
	rm -r $(BUILD_DIR)
	rm -f $(OUTFILE)
ifneq ($(NO_DEPS), true)
	rm -r $(DEPS_DIR)
endif

help:
	@echo "Demo space game Makefile: make [target] [options]"
	@echo "  - Available targets: dirs, compile, link, run, clean, help, all (default, does dirs, compile and link)"
	@echo "  - Available options:"
	@echo "    - address=true - turn on address sanitizer"
	@echo "    - RAYGUI=<path> - specify custom path to the raygui.h header"
	@echo "        (note: default expected raygui path is ../../raygui/src/raygui.h)"
	@echo "    - RAYLIB_PATH=<path> - specify path to custom portable raylib location"
	@echo "        (note: expects a path to the 'raylib' folder without trailing slash, containing lib and include)"
	@echo "        (note: by default, this flag isn't set and the makefile expects raylib installed in the system)"
	@echo "    - NO_DEPS=true - turn off additional dependency targets"


-include $(DEPS_DIR)/*.dep
