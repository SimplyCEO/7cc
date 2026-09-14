ESCAPE        := \033
YELLOW				:= $(ESCAPE)[33m
GREEN         := $(ESCAPE)[32m
BOLD_GREEN    := $(ESCAPE)[1;32m
BLUE          := $(ESCAPE)[34m
BOLD_BLUE     := $(ESCAPE)[1;34m
RESET_COLOUR  := $(ESCAPE)[0m

PWD           := $(shell pwd | sed 's/ /\\ /g')
INTERNAL_DIR	:= lib logic include internal vendor
OBJ_DIR       := $(PWD)/Config
SRC_DIR       := $(PWD)/src
DIRS          := Config

_SOURCES      := $(shell find $(SRC_DIR) $(foreach d,$(INTERNAL_DIR),-path "$(SRC_DIR)/$(d)" -prune -o) -name "*.lua" -print)
_SOURCES      := $(patsubst $(SRC_DIR)/%,%,$(_SOURCES))
_OBJECTS      := $(patsubst %.lua,%.xml,$(_SOURCES))

SOURCES       := $(addprefix $(SRC_DIR)/, $(_SOURCES))
OBJECTS       := $(addprefix $(OBJ_DIR)/, $(_OBJECTS))

.PHONY: all

all: directories lang $(OBJECTS)

$(OBJ_DIR)/%.xml: $(SRC_DIR)/%.lua
	@printf "[7CC] $(GREEN)Building XML object '%s'$(RESET_COLOUR)\n" "$@"
	@7cc -i 1 -c -o $@ $<

lang:
	@if [ ! -f src/lang.csv ]; then printf "[7CC] $(YELLOW)CSV object '%s' not found.$(RESET_COLOUR)\n" "$(PWD)/src/lang.csv"; else printf "[7CC] $(YELLOW)Shipping CSV object '%s'$(RESET_COLOUR)\n" "$(PWD)/Config/Localization.csv"; cp src/lang.csv Config/Localization.csv; fi

directories: $(DIRS)

$(DIRS):
	@printf "[DIR] ""$(BLUE)""Directory ""$(BOLD_BLUE)""'$@'""$(RESET_COLOUR)$(BLUE)"" created""$(RESET_COLOUR).""\n"
	@mkdir -p $(PWD)/$@

clean:
	@rm -rv $(OBJ_DIR) || true

