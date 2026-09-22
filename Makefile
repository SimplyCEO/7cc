# GENERAL
CC          := clang
OSNAME      := unix32
BUILD_TYPE  := Debug
BUILTIN_LUA := 1
BUILD64     := 0

ifeq ($(OSNAME),unix64)
	BUILD64 = 1
endif

ifeq ($(OSNAME),win64)
	BUILD64 = 1
endif

# DIRECTORIES
SRC_DIR        := src
OBJ_DIR        := .obj
BIN_DIR        := bin
INSTALL_PREFIX := /usr/local
PWD            := $(shell pwd)

# FILES
SOURCES := $(shell find $(SRC_DIR) -name '*.c' | sort)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
DIRS    := $(shell echo $(OBJECTS) | tr ' ' '\n' | xargs -n1 dirname | sort -u) bin
TARGETS := 7cc

# COMPILER AND LINKER
CFLAGS    := -Wall -Wextra -Werror
HEADERS   := -I./src/core -I./src/lua -I./src/lua/l_field -I./src/skel -I./src/xml -I./vendor/lua
LIBRARIES := -llua -lm
LDFLAGS   :=

ifeq ($(CC),tcc)
	CFLAGS += -std=c89
else
	CFLAGS += --std=c89
endif

ifeq ($(BUILD_TYPE),Release)
	ifneq ($(CC),tcc)
		CFLAGS += -O2
	endif
else ifeq ($(BUILD_TYPE),Debug)
	ifneq ($(CC),tcc)
		CFLAGS += -O0
	endif
	CFLAGS += -g3 -ggdb -Wall
endif

ifeq ($(BUILD64),0)
	CFLAGS += -m32 -DLUA_32BITS
	LDFLAGS += -m32
endif

ifeq ($(BUILTIN_LUA),1)
	LDFLAGS += -L./vendor/lua/$(OSNAME)
endif

# COLOUR ESCAPE CODES
GREEN        := \033[32m
BOLD_GREEN   := \033[1;32m
BLUE         := \033[34m
BOLD_BLUE    := \033[1;34m
RESET_COLOUR := \033[0m

.PHONY: all

all: directories $(TARGETS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@printf "[CC] $(GREEN)Building object '%s'$(RESET_COLOUR)\n" "$<"
	@$(CC) -static $(CFLAGS) $(HEADERS) -c $< -o $@

$(TARGETS): $(OBJECTS)
	@printf "[LD] $(BOLD_GREEN)Linking binary '%s'$(RESET_COLOUR)\n" "$@"
	@$(CC) -static $^ $(LDFLAGS) $(LIBRARIES) $(HEADERS) -o $(BIN_DIR)/$@

directories: $(DIRS)

$(DIRS):
	@printf "[DIR] $(BLUE) Directory $(BOLD_BLUE)'%s'$(RESET_COLOUR)$(BLUE) created$(RESET_COLOUR).\n" "$@"
	@mkdir -p $(PWD)/$@

install: $(TARGETS)
	@mkdir -p $(INSTALL_PREFIX)/bin
	@install -m 755 $(BIN_DIR)/7cc $(INSTALL_PREFIX)/bin

clean:
	@rm -rv $(BIN_DIR) $(OBJ_DIR) build 2>/dev/null || true

