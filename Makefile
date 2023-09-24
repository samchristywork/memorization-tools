CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c++11 -Isrc/

all: build/main

build/%.o: src/%.cpp
	mkdir -p build
	$(CC) -c $(CFLAGS) $< -o $@

build/main: build/util.o build/cloze.o build/firstletter.o build/main.o
	${CC} $(CFLAGS) build/*.o ${LIBS} -o $@

run: build/main
	./build/main firstletter material.txt

.PHONY: clean
clean:
	rm -rf build
