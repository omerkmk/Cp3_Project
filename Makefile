CC = gcc
CFLAGS = -Wall -Iinclude

all: train_loading_system

train_loading_system: src/main.c src/train.c src/wagon.c src/material.c src/file_ops.c src/utils.c
	$(CC) $(CFLAGS) -o train_loading_system src/main.c src/train.c src/wagon.c src/material.c src/file_ops.c src/utils.c

clean:
	rm -f train_loading_system
