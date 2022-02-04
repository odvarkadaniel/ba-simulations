#ifndef PARITY_GAME_H
#define PARITY_GAME_H

#include"automaton.h"
#include<iostream>

template<typename State, typename Symbol>
class parityGame : protected Automaton {
    public:
        void fillTransition(std::multimap<std::pair<int, int>, std::string>& transitions, std::vector<int> states, std::vector<std::string> alpha);
        void constructv0F(Automaton a);
        void constructv1F(std::vector<int> &states);
        void constructFPG(Automaton omega, std::multimap<std::pair<int, int>, std::string> &transitions);

        typedef struct gameBoard {
            
        }sFPGBoard;

    protected:
        std::multimap<std::pair<int, int>, std::string> v0F; // just like a transition
        std::vector<std::pair<State, State>> v1F;
        std::pair<int, int> EFA;

};



template <typename State, typename Symbol>
void parityGame<State, Symbol>::fillTransition(std::multimap<std::pair<int, int>, std::string>& transitions, std::vector<int> states, std::vector<std::string> alpha) {
    using namespace std;
    int f = 0;
    for(const auto &v1 : v1F) {
        f = 0;
        for(const auto& alp : alpha) {
            f = 0;
            for(int i = 0; i < states.size(); i++) {
                if(f) break;
                for(const auto& [p, val] : transitions) {
                    if(f) break;
                    if(p.first == states[i] && val == alp && p.second == v1.first) {
                        v0F.insert(make_pair(make_pair(v1.first, v1.second), alp));
                        f = 1;
                        break;
                        }
                }
            }
        }
    }

    return;
}

template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructv0F(Automaton a) {
    using namespace std;
    auto transition = a.getTransitions();
    auto states = a.getStates();
    auto alphabet = a.getAlphabet();
    fillTransition(transition, states, alphabet);

    return;
}

template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructv1F(std::vector<int> &states) {
    using namespace std;

    for(auto &x : states) {
        for(auto &y : states) {
            v1F.push_back(make_pair(x, y));
        }
    }
    return;
}


template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructFPG(Automaton omega, std::multimap<std::pair<int, int>, std::string> &transitions) {
    using namespace std;

    // construct V1F accordingly
    vector<int> states = omega.getStates();
    constructv1F(states);

    // construct V0F
    constructv0F(omega);
    
    #ifdef _DEBUG
    cout << "v0F: \n";
    for(const auto& elem : v0F) {
        cout << elem.first.first << " " << elem.second << " " << elem.first.second << "\n";
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