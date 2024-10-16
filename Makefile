HOME		= .
SRC			= $(HOME)/src
BIN			= $(HOME)/bin
LIB			= $(HOME)

SOURCES		= $(wildcard $(SRC)/*.c)
BINARIES	= $(patsubst $(SRC)/%.c, $(BIN)/%, $(SOURCES))

CC			= gcc
CCFLAGS		= -I$(LIB)

all: dirs $(BINARIES)

$(BIN)/%: $(SRC)/%.c $(LIB)/bitmap.c
	@echo "- compiling $<..."
	@$(CC) $(CCFLAGS) -o $@ $^

.PHONY: dirs
dirs:
	@mkdir -p $(BIN)

.PHONY: clean
clean:
	@rm -rf $(BIN)
