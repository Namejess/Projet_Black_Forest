CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL2 -lSDL2_image

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: Projet_Black_Forest

Projet_Black_Forest: main.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -Iinclude

clean:
	rm -f main.o $(OBJ) Projet_Black_Forest
