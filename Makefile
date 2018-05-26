CC=gcc
CFLAGS=-Wall -pedantic
DEPS=server.h
OBJ=build/server.o build/main.o
EXEC=server

all: $(OBJ) buildDir
	$(CC) $(CFLAGS) -o build/$(EXEC) $(OBJ)

build/%.o: %.c $(DEPS) buildDir
	$(CC) $(CFLAGS) -c -o $@ $<

buildDir:
	-mkdir build

clean:
	rm -r build/
