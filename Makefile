CXX = g++
CFLAGS = -std=c++17 -I. -Wzero-as-null-pointer-constant

all: main
files = src/automaton.cpp src/util.cpp

build: src/main.cpp src/automaton.cpp src/simulations.h src/util.cpp src/util.h src/automaton.h src/parity_game.h
	$(CXX) src/main.cpp $(CFLAGS) -o main $(files)

debug: src/main.cpp src/automaton.cpp src/simulations.h src/util.cpp src/util.h src/automaton.h src/parity_game.h
	$(CXX) src/main.cpp -g $(CFLAGS) -o main $(files) -D_DEBUG

debug2: src/main.cpp src/automaton.cpp src/simulations.h src/util.cpp src/util.h src/automaton.h src/parity_game.h
	$(CXX) src/main.cpp -g $(CFLAGS) -o main $(files) -D_DEBUG

run:
	make build
	./main

rund:
	make debug
	./main

clean:
	rm -f *.o main
