CXX = g++
CFLAGS = -std=c++17 -I.

all: main
files = simulations.cpp automaton.cpp util.cpp

make: main.cpp automaton.cpp automaton.h simulations.cpp simulations.h util.cpp util.h
	$(CXX) main.cpp $(CFLAGS) -o main $(files)

debug: main.cpp automaton.cpp simulations.cpp simulations.h util.cpp util.h automaton.h
	$(CXX) main.cpp $(CFLAGS) -o main $(files) -D_DEBUG

run:
	make
	./main

rund:
	debug
	./main

clean:
	rm -f *.o main
