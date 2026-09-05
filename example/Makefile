ESCAPE        := \033
GREEN         := $(ESCAPE)[32m
BOLD_GREEN    := $(ESCAPE)[1;32m
BLUE          := $(ESCAPE)[34m
BOLD_BLUE     := $(ESCAPE)[1;34m
RESET_COLOUR  := $(ESCAPE)[0m

PWD           := $(shell pwd | sed 's/ /\\ /g')
OBJ_DIR       := $(PWD)/obj
SRC_DIR       := $(PWD)/src
DIRS          := obj

_SOURCES      := $(shell find $(SRC_DIR) -name "*.lua")
_SOURCES      := $(patsubst $(SRC_DIR)/%,%,$(_SOURCES))
_OBJECTS      := $(patsubst %.lua,%.xml,$(_SOURCES))

SOURCES       := $(addprefix $(SRC_DIR)/, $(_SOURCES))
OBJECTS       := $(addprefix $(OBJ_DIR)/, $(_OBJECTS))

.PHONY: all

all: directories $(OBJECTS)

$(OBJ_DIR)/%.xml: $(SRC_DIR)/%.lua
	@printf "[7CC] ""$(GREEN)""Building xml object '$<'""$(RESET_COLOUR)""\n"
	@7cc -i 1 -c $< -o $@

directories: $(DIRS)

$(DIRS):
	@printf "[DIR] ""$(BLUE)""Directory ""$(BOLD_BLUE)""'$@'""$(RESET_COLOUR)$(BLUE)"" created""$(RESET_COLOUR).""\n"
	@mkdir -p $(PWD)/$@

clean:
	@rm -rv $(OBJ_DIR) 2>/dev/null || true

