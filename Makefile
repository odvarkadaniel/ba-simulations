# -Wzero-as-null-pointer-constant

CXX = g++
CFLAGS = -std=c++17 -I. -O2

files = src/automaton.cpp src/util.cpp

all: src/main.cpp src/automaton.cpp src/simulations.h src/util.cpp src/util.h src/automaton.h src/fair_parity_game.h
	$(CXX) src/main.cpp $(CFLAGS) -o main $(files)

build: src/main.cpp src/automaton.cpp src/simulations.h src/util.cpp src/util.h src/automaton.h src/fair_parity_game.h
	$(CXX) src/main.cpp $(CFLAGS) -o main $(files)

debug: src/main.cpp src/automaton.cpp src/simulations.h src/util.cpp src/util.h src/automaton.h src/fair_parity_game.h
	$(CXX) src/main.cpp -g $(CFLAGS) -o main $(files) -D_DEBUG

debug2: src/main.cpp src/automaton.cpp src/simulations.h src/util.cpp src/util.h src/automaton.h src/fair_parity_game.h
	$(CXX) src/main.cpp -g $(CFLAGS) -o main $(files) -D_DEBUG

test: src/automaton.cpp src/simulations.h src/util.cpp src/util.h src/automaton.h src/fair_parity_game.h
	$(CXX) tests/tests.cpp $(CFLAGS) -o test_all $(files) -lgtest -lpthread

run:
	make build
	./main

rund:
	make debug
	./main

clean:
	rm -f *.o main test_all
