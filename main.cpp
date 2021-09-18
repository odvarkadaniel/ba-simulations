#include<stdlib.h>

#include"simulations.h"

int main(int argc, char *argv[]) {
    using namespace std;

    Automaton a;
    a = a.loadAutomaton("omega.aut");

    Simulation s;
    
    vector<pair<int, int>> omega = s.directSimulationRelation(a);

    return 0; 
}