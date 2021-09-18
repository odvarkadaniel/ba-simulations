CXX = g++
CFLAGS = -stc=c++17 -I.

all: main

build: main.cpp automaton.cpp automaton.h simulations.cpp simulations.h util.cpp util.h
	$(GXX) main.cpp $(CFLAGS) -o main automaton.cpp simulations.cpp util.cpp

debug: main.cpp automaton.cpp simulations.cpp simulations.h util.cpp util.h automaton.h
	$(GXX) main.cpp -o $(CFLAGS) main main.o automaton.o simulations.o util.o -D_DEBUG

run:
	make
	./main

rund:
	debug
	./main

clean:
	rm -f *.o main
