#!/bin/bash

if [ $1 == "debug" ] 
then
	echo "Debug version compilation..."	
	g++ main.cpp -std=c++17 automaton.cpp simulations.cpp util.cpp -D_DEBUG -o main
else
	g++ main.cpp -std=c++17 automaton.cpp simulations.cpp util.cpp -o main
fi
