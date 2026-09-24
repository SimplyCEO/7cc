# GENERAL
CC           := clang
OSARCH       := i386
OSNAME       := unix32
OSTYPE       := linux
BUILD_TYPE   := Release
BUILTIN_LIBC := 1
BUILTIN_LUA  := 1
BUILD64      := 0

ifeq ($(OSNAME),unix32)
	OSARCH  = i386
	OSTYPE  = linux
	BUILD64 = 0
endif

ifeq ($(OSNAME),unix64)
	OSARCH  = amd64
	OSTYPE  = linux
	BUILD64 = 1
endif

ifeq ($(OSNAME),mingw32)
	CC           = gcc
	OSARCH       = i386
	OSTYPE       = mingw
	BUILTIN_LIBC = 0
	BUILD64      = 0
endif

ifeq ($(OSNAME),mingw64)
	CC           = gcc
	OSARCH       = amd64
	OSTYPE       = mingw
	BUILTIN_LIBC = 0
	BUILD64      = 1
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
CFLAGS    := -DBUILD64=$(BUILD64) -DLUA_32BITS -Wall -Wextra
HEADERS   := -I./src/core -I./src/lua -I./src/lua/l_field -I./src/skel -I./src/xml
LIBRARIES := -llua -lm
LDFLAGS   :=

ifneq ($(CC),gcc)
	CFLAGS += -Werror
endif

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
	CFLAGS += -m32
	LDFLAGS += -m32
endif

ifeq ($(BUILTIN_LIBC),1)
	HEADERS += -I./vendor/musl/$(OSARCH)-musl-$(OSTYPE)/include
	LDFLAGS += -static ./vendor/musl/lua-compatibility.c -L./vendor/musl/$(OSARCH)-musl-$(OSTYPE)/lib
endif

ifeq ($(BUILTIN_LUA),1)
	HEADERS += -I./vendor/lua/include
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
	@$(CC) $(HEADERS) -c $< $(CFLAGS) -o $@

$(TARGETS): $(OBJECTS)
	@printf "[LD] $(BOLD_GREEN)Linking binary '%s'$(RESET_COLOUR)\n" "$@"
	@$(CC) $(HEADERS) $^ $(LDFLAGS) $(LIBRARIES) -o $(BIN_DIR)/$@

directories: $(DIRS)

$(DIRS):
	@printf "[DIR] $(BLUE) Directory $(BOLD_BLUE)'%s'$(RESET_COLOUR)$(BLUE) created$(RESET_COLOUR).\n" "$@"
	@mkdir -p $(PWD)/$@

install:
	@mkdir -p $(INSTALL_PREFIX)/bin
	@install -m 755 $(BIN_DIR)/7cc $(INSTALL_PREFIX)/bin

clean:
	@rm -rv $(BIN_DIR) $(OBJ_DIR) build 2>/dev/null || true

