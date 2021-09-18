CXX = g++
CFLAGS = -pedantic -Wall -Wextra -O2 -I

all: main

build: main.cpp automaton.cpp util.cpp util.h automaton.h
	$(GXX) main.cpp -o main

debug: main.cpp automaton.cpp util.cpp util.h automaton.h
	$(GXX) main.cpp -o main -D_DEBUG

run:
	make
	./main

rund:
	debug
	./main

clean:
	rm -f *.o main