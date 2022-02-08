#include<iostream>

#include"simulations.h"
#include"parity_game.h"

int main(int argc, char *argv[]) {
    using namespace std;

    Automaton<string, string> a;
    a = a.loadAutomaton("omega.ba");

    Simulation<string, string> s;
    set<pair<string, string>> omega = s.directSimulationRelation(a);

    Delta<std::string, std::string> transitions = a.getTransitions();
    parityGame<string, string> pg;
    pg.constructFPG(a, transitions);
    
    return 0; 
}