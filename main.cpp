#include<stdlib.h>
#include<iostream>

#include"simulations.h"
#include"parity_game.h"

int main(int argc, char *argv[]) {
    using namespace std;

    Automaton a;
    a = a.loadAutomaton("omega.ba");

    //Simulation s;
    //vector<pair<int, int>> omega = s.directSimulationRelation(a);


    std::multimap<std::pair<int, int>, std::string> transitions = a.getTransitions();
    parityGame<int, string> pg;
    pg.constructFPG(a, transitions);
    
    return 0; 
}