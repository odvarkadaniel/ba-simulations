#ifndef PARITY_GAME_H
#define PARITY_GAME_H

#include"automaton.h"
#include<iostream>

template<typename State, typename Symbol>
class parityGame : protected Automaton<std::string, std::string> {
    public:
        void constructv0F(Automaton<State, Symbol> a);
        void constructv1F(std::set<std::string> &states);
        void constructFPG(Automaton<State, Symbol> omega, std::map<std::pair<State, Symbol>, std::set<std::string>> &transitions);

        typedef struct gameBoard {
            
        }sFPGBoard;

    protected:
        std::map<std::pair<State, Symbol>, std::set<std::string>> v0F; // just like a transition
        std::set<std::pair<State, State>> v1F;
        std::pair<int, int> EFA;

};

template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructv0F(Automaton<State, Symbol> a) {
    using namespace std;
    auto transition = a.getTransitions();
    auto states = a.getStates();
    auto alphabet = a.getAlphabet();
    //fillTransition(transition, states, alphabet);


    for(const auto& st : states) {
        for(const auto& v1 : v1F) {
            for(const auto& a : alphabet) {
                for(const auto& [p, val] : transition) {
                    for(const auto& setval : val) {
                        if((st == p.first && a == p.second && v1.first == setval)) {
                            v0F[make_pair(v1.first, a)].insert(v1.second);
                        }
                    }
                }
            }
        }
    }


    return;
}

template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructv1F(std::set<std::string> &states) {
    using namespace std;

    for(auto &x : states) {
        for(auto &y : states) {
            v1F.insert({make_pair(x, y)});
        }
    }
    return;
}


template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructFPG(Automaton<State, Symbol> omega, std::map<std::pair<State, Symbol>, std::set<std::string>> &transitions) {
    using namespace std;

    // construct V1F accordingly
    set<string> states = omega.getStates();
    constructv1F(states);

    // construct V0F
    constructv0F(omega);
    
    #ifdef _DEBUG
    cout << "v0F: \n";
    for(const auto& elem : v0F) {
        for(const auto& e : elem.second) {
            std::cout << elem.first.first << " " << elem.first.second << " " << e << endl;
        }
    }
    cout << endl;
    cout << "v1F: \n";
    for(const auto &x : v1F) {
        cout << x.first << " " << x.second << endl;
    }
    #endif 

    return;
}

#endif