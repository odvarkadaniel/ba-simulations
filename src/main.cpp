#include<iostream>
#include<getopt.h>

#include"simulations.h"
#include"fair_parity_game.h"
#include"fair_parity_game_solver.h"
#include"delayed_parity_game.h"
#include"delayed_parity_game_solver.h"

int main(int argc, char *argv[]) {
    using namespace std;

    Automaton<string, string> a;

    bool direct = false, fair = false, delayed = false, dot = false, print = false;

    int c;
    while(1) {
        int optionIndex = 0;
        static struct option long_options[] = {
            {"file", required_argument, 0, 's'},
            {"direct", no_argument, 0, 'd'},
            {"fair", no_argument, 0, 'f'},
            {"delayed", no_argument, 0, 'z'},
            {"dot", no_argument, 0, 1}, // TODO
            {"help", no_argument, 0, 'h'},
            {"print", no_argument, 0, 'p'},
        };

        c = getopt_long(argc, argv, "s:fdzh", long_options, &optionIndex);
        if(c == -1)
            break;

        switch(c) {
            case 1:
                dot = true;
                break;
            case 's':
                a = a.loadAutomaton(string(optarg));
                break;
            case 'd':
                direct = true;
                break;
            case 'f':
                fair = true;
                break;
            case 'z':
                delayed = true;
                break;
            case 'h':
                printHelp();
                break;
            case 'p':
                print = true;
                break;
            case '?':
                printHelp();
                break;
        }
    }

    if(direct) {
        Simulation<string, string> s;
        if(dot)
            s.dotfile = true;
        set<pair<string, string>> omega = s.directSimulationRelation(a);
    } else if(fair) {
        Delta<std::string, std::string> transitions = a.getTransitions();
        fairParityGame<string, string> fpg;
        fpg.constructFPG(a, transitions);
        parityGameSolver<fairParityGame<string, string>, string, string> pgsolver;
        auto result = pgsolver.solveParityGame(fpg, a);
        if(print) {
            for(const auto &pairDPG : result) {
                std::cout << "(" << pairDPG.first << ", " << pairDPG.second << ")\n";
            }
        }
    } else if(delayed) {
        delayedParityGame<string, string> dpg;
        dpg.constructDPG(a);
        delayedParityGameSolver<delayedParityGame<string, string>, string, string> pgsolver;
        auto result = pgsolver.solveParityGame(dpg, a);
        if(print) {
            for(const auto &pairDPG : result) {
                std::cout << "(" << pairDPG.first << ", " << pairDPG.second << ")\n";
            }
        }
    } else {
        cerr << "No simulation to compute selected\n\n";
        printHelp();
    }
    
    return 0; 
}