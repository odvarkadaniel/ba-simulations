CXX = g++
CFLAGS = -std=c++17 -I. -Wzero-as-null-pointer-constant

all: main
files = automaton.cpp util.cpp

build: main.cpp automaton.cpp simulations.h util.cpp util.h automaton.h parity_game.h
	$(CXX) main.cpp $(CFLAGS) -o main $(files)

debug: main.cpp automaton.cpp simulations.h util.cpp util.h automaton.h parity_game.h
	$(CXX) main.cpp -g $(CFLAGS) -o main $(files) -D_DEBUG

debug2: main.cpp automaton.cpp simulations.h util.cpp util.h automaton.h parity_game.h
	$(CXX) main.cpp -g $(CFLAGS) -o main $(files) -D_DEBUG

run:
	make build
	./main

rund:
	make debug
	./main

clean:
	rm -f *.o main
