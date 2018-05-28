CC=gcc
CFLAGS=-std=c11 -g -Wall -pedantic -pthread
DEPS=server.h parse_args.h
OBJ=build/server.o build/main.o build/parse_args.o
EXEC=server

all: $(OBJ) buildDir
	$(CC) $(CFLAGS) -o build/$(EXEC) $(OBJ)

build/%.o: %.c $(DEPS) buildDir
	$(CC) $(CFLAGS) -c -o $@ $<

buildDir:
	-mkdir build

clean:
	rm -r build/
