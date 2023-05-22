CC = gcc
CFLAGS = -Wall -g

SRC = tema3.c trie.c library.c
EXE = tema3 

build:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

run:
	./$(EXE)

.PHONY: clean

clean:
	rm -rf $(EXE)
