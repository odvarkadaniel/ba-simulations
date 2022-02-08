#include<iostream>
#include<getopt.h>

#include"simulations.h"
#include"parity_game.h"

int main(int argc, char *argv[]) {
    using namespace std;

    Automaton<string, string> a;

    bool direct = false, fair = false, delayed = false, dot = false;

    int c;
    while(1) {
        int optionIndex = 0;
        static struct option long_options[] = {
            {"file", required_argument, 0, 's'},
            {"direct", no_argument, 0, 'd'},
            {"fair", no_argument, 0, 'f'},
            {"delayed", no_argument, 0, 'z'},
            {"dot", no_argument, 0, 1},
            {"help", no_argument, 0, 'h'},
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
        parityGame<string, string> pg;
        pg.constructFPG(a, transitions);
    } else if(delayed) {
        cerr << "Delayed simulation not implement yet\n";
    } else {
        cerr << "No simulation to compute selected\n\n";
        printHelp();
    }
    
    return 0; 
}