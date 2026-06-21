CC      = gcc
CFLAGS  = -Wall -Werror -Wextra -pedantic -std=gnu11
LDFLAGS = -lm
SRC     = visualizer.c algorithms.c main.c
OBJ     = $(SRC:.c=.o)
BIN     = search_viz

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.c visualizer.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(OBJ) $(BIN)

re: clean all

.PHONY: all run clean re
