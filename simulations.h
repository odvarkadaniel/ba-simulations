#ifndef SIMULATIONS_H
#define SIMULATIONS_H

#include"automaton.h"

class Simulation {
    public:
        std::vector<std::pair<int, int>> directSimulationRelation(Automaton &a);

};

#endif