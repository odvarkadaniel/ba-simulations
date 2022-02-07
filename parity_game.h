#ifndef PARITY_GAME_H
#define PARITY_GAME_H

#include"automaton.h"
#include<iostream>
#include<tuple>

template<typename State, typename Symbol>
class parityGame : protected Automaton<State, Symbol> {
    public:
        void constructv0F(Automaton<State, Symbol> a);
        void constructv1F(std::set<State> &states);
        void constructFPG(Automaton<State, Symbol> omega, std::map<std::pair<State, Symbol>, std::set<std::string>> &transitions);
        int getPriorityv0F(std::set<State> acceptingState, State v0);
        int getPriorityv1F(std::set<State> acceptingState, State v1f, State v1s);

    protected:
        std::set<std::tuple<State, State, Symbol, int>> v0F;
        std::set<std::tuple<State, State, int>> v1F;
        //std::pair<int, int> EFA;

};

template<typename State, typename Symbol>
int parityGame<State, Symbol>::getPriorityv1F(std::set<State> acceptingState, State v1f, State v1s) {
    using namespace std;

    for(const auto& accSt : acceptingState) {
        for (const auto &v0: v0F) {
            if (get<1>(v0) == v1s && get<1>(v0) == accSt) {
                return 0;
            } else if(acceptingState.find(v1f) != acceptingState.end() && acceptingState.find(v1s) == acceptingState.end()) {
                return 1;
            }
        }
    }
    return 2;
}

template<typename State, typename Symbol>
int parityGame<State, Symbol>::getPriorityv0F(std::set<State> acceptingState, State v0) {
    using namespace std;

    for(const auto& accSt : acceptingState) {
        for (const auto &v1: v1F) {
            if (get<1>(v1) == v0 && get<1>(v1) == accSt) {
                return 0;
            }
        }
    }
    return 2;
}

template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructv0F(Automaton<State, Symbol> a) {
    using namespace std;
    auto transition = a.getTransitions();
    auto states = a.getStates();
    auto alphabet = a.getAlphabet();

    for(const auto& st : states) {
        for(const auto& v1 : v1F) {
            for(const auto& a : alphabet) {
                for(const auto& [p, val] : transition) {
                    for(const auto& setval : val) {
                        if((st == p.first && a == p.second && get<0>(v1) == setval)) {
                            v0F.insert(make_tuple(get<0>(v1), get<1>(v1), a, -1));
                        }
                    }
                }
            }
        }
    }


    return;
}

template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructv1F(std::set<State> &states) {
    using namespace std;

    for(auto &x : states) {
        for(auto &y : states) {
            v1F.insert({make_tuple(x, y, -1)});
        }
    }
    return;
}


template <typename State, typename Symbol>
void parityGame<State, Symbol>::constructFPG(Automaton<State, Symbol> omega, std::map<std::pair<State, Symbol>, std::set<std::string>> &transitions) {
    using namespace std;

    // construct V1F accordingly
    set<State> states = omega.getStates();
    set<State> acceptingStates = omega.getAcceptingStates();

    constructv1F(states);

    // construct V0F
    constructv0F(omega);

    for(const auto& v0 : v0F) {
        int p = getPriorityv0F(acceptingStates, get<1>(v0));
        v0F.erase(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), -1));
        v0F.insert(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), p));
    }

    for(const auto& v1 : v1F) {
        int p = getPriorityv1F(acceptingStates, get<0>(v1), get<1>(v1));
        v1F.erase(make_tuple(get<0>(v1), get<1>(v1), -1));
        v1F.insert(make_tuple(get<0>(v1), get<1>(v1), p));
    }

    #ifdef _DEBUG
    cout << "v0F: \n";
    for (const auto& x : v0F) {
        cout << get<0>(x) << ' ' << get<1>(x) << ' ' << get<2>(x) << ' ' << get<3>(x) << '\n';
    }

    cout << endl;

    cout << "v1F: \n";
    for(const auto &x : v1F) {
        cout << get<0>(x) << ' ' << get<1>(x) << ' ' << get<2>(x) << '\n';
    }
    #endif

    return;
}

#endif