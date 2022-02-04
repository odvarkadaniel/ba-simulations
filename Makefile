CXX = g++
CFLAGS = -std=c++17 -I. -Wzero-as-null-pointer-constant

all: main
files = simulations.cpp automaton.cpp util.cpp

build: main.cpp automaton.cpp simulations.cpp simulations.h util.cpp util.h automaton.h parity_game.h
	$(CXX) main.cpp $(CFLAGS) -o main $(files)

debug: main.cpp automaton.cpp simulations.cpp simulations.h util.cpp util.h automaton.h parity_game.h
	$(CXX) main.cpp $(CFLAGS) -o main $(files) -D_DEBUG

run:
	make build
	./main

rund:
	make debug
	./main

clean:
	rm -f *.o main
