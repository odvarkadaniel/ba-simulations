#include<iostream>
#include<getopt.h>

#include"simulations.h"
#include"fair_parity_game.h"
#include"fair_parity_game_solver.h"
#include"delayed_parity_game.h"
#include"delayed_parity_game_solver.h"
#include"fair_solver_fast.h"
#include"delayed_solver_fast.h"

int main(int argc, char *argv[]) {
    using namespace std;

    Automaton<string, string> a;

    bool direct = false, fair = false, delayed = false, dot = false, print = false, fast = false;

    int c;
    while(1) {
        int optionIndex = 0;
        static struct option long_options[] = {
            {"file", required_argument, 0, 's'},
            {"direct", no_argument, 0, 'd'},
            {"fair", no_argument, 0, 'f'},
            {"delayed", no_argument, 0, 'z'},
            {"dot", no_argument, 0, 1},
            {"fast", no_argument, 0, 2},
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
            case 2:
                fast = true;
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
                return 0;
            case 'p':
                print = true;
                break;
            case '?':
                printHelp();
                return 1;
        }
    }

    if(direct) {
        Simulation<string, string> s;
        if(dot)
            s.dotfile = true;
        auto result = s.directSimulationRelation(a);
        if(print) {
            for(const auto &pairDPG : result) {
                std::cout << "(" << pairDPG.first << ", " << pairDPG.second << ")\n";
            }
        }
        if(dot) {
            auto dotString = printAutAsDot(a, result);
        }
        std::cout << result.size() << "\n";
    } else if(fair) {
        Delta<std::string, std::string> transitions = a.getTransitions();
        fairParityGame<std::string, std::string> fpg;
        fpg.constructFPG(a, transitions);
        if(fast) {
            fastFairSolver<fairParityGame<std::string, std::string>, string, string> fastpgsolver;
            auto result = fastpgsolver.fast(fpg, a);
            if(print) {
                for(const auto &pairDPG : result) {
                    std::cout << "(" << pairDPG.first << ", " << pairDPG.second << ")\n";
                }
                std::cout << "number of states simulating: " << result.size() << "\n";
                if(dot) {
                    auto dotString = printAutAsDot(a, result);
                }
            }
            std::cout << result.size() << "\n";
        } else {
            parityGameSolver<fairParityGame<string, string>, string, string> pgsolver;
            auto result = pgsolver.solveParityGame(fpg, a);
            if(print) {
                for(const auto &pairDPG : result) {
                    std::cout << "(" << pairDPG.first << ", " << pairDPG.second << ")\n";
                }
            }
            if(dot) {
                auto dotString = printAutAsDot(a, result);
            }
            std::cout << result.size() << "\n";
        }
    } else if(delayed) {
        delayedParityGame<string, string> dpg;
        dpg.constructDPG(a);
        if(fast) {
            fastDelayedSolver<delayedParityGame<string, string>, string, string> pgsolver;
            auto result = pgsolver.fast(dpg, a);
            if(print) {
                for(const auto &pairDPG : result) {
                    std::cout << "(" << pairDPG.first << ", " << pairDPG.second << ")\n";
                }
            }
            if(dot) {
                auto dotString = printAutAsDot(a, result);
            }
            std::cout << result.size() << "\n";
        } else {
            delayedParityGameSolver<delayedParityGame<string, string>, string, string> pgsolver;
            auto result = pgsolver.solveParityGame(dpg, a);
            if(print) {
                for(const auto &pairDPG : result) {
                    std::cout << "(" << pairDPG.first << ", " << pairDPG.second << ")\n";
                }
            }
            if(dot) {
                auto dotString = printAutAsDot(a, result);
            }
            std::cout << result.size() << "\n";
        }
    } else {
        cerr << "No simulation to compute selected\n\n";
        printHelp();
    }
    
    return 0; 
}