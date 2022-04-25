#ifndef PARITY_GAME_H
#define PARITY_GAME_H

#include<iostream>
#include<tuple>

/**
 * Class that holds the parity game parts
 * @tparam State
 * @tparam Symbol
 */
template<typename State, typename Symbol>
class fairParityGame : protected Automaton<State, Symbol> {
    public:
        std::set<std::tuple<State, State, Symbol, int, int>> getv0F() { return this->m_v0F; };
        std::set<std::tuple<State, State, int, int>> getv1F() { return this->m_v1F; };
        std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &getEFA0() { return this->m_EFA0; };
        std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &getEFA1() { return this->m_EFA1; };

        void constructm_v0F(Automaton<State, Symbol> a);
        void constructm_v1F(std::set<State> &states);
        void constructFPG(Automaton<State, Symbol> &omega, std::map<std::pair<State, Symbol>, std::set<std::string>> &transitions);
        int getPrioritym_v0F(std::set<State> &acceptingState, State v0Second);
        int getPrioritym_v1F(std::set<State> &acceptingState, State m_v1F, State v1s);
        void constructEFA(std::map<std::pair<State, Symbol>, std::set<State>>& transitions);

        std::set<std::tuple<State, State, int, int>> succ0(std::tuple<State, State, Symbol, int, int> v0, std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0);
        std::set<std::tuple<State, State, Symbol, int, int>> succ1(std::tuple<State, State, int, int> v1, std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1);

    private:
        std::set<std::tuple<State, State, Symbol, int, int>> m_v0F; // set with Player 0's states
        std::set<std::tuple<State, State, int, int>> m_v1F; // set with Player 1's states
        std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> m_EFA0;
        std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> m_EFA1;

};

template<typename State, typename Symbol>
std::set<std::tuple<State, State, int, int>> fairParityGame<State, Symbol>::succ0(std::tuple<State, State, Symbol, int, int> v0, std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0) {
    std::set<std::tuple<State, State, int, int>> result;
     for(auto &tran : EFA0) {
         if(std::get<0>(tran.first) == std::get<0>(v0) &&
            std::get<1>(tran.first) == std::get<1>(v0) &&
            std::get<2>(tran.first) == std::get<2>(v0) &&
            std::get<3>(tran.first) == std::get<3>(v0)) {
             for(auto& sec : tran.second) {
                 result.insert(std::make_tuple(std::get<0>(sec), std::get<1>(sec), std::get<2>(sec), std::get<3>(sec)));
             }
         }
     }
    return result;
}

template<typename State, typename Symbol>
std::set<std::tuple<State, State, Symbol, int, int>> fairParityGame<State, Symbol>::succ1(std::tuple<State, State, int, int> v1, std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1) {
    std::set<std::tuple<State, State, Symbol, int, int>> result;
    for(auto &tran : EFA1) {
        if(std::get<0>(tran.first) == std::get<0>(v1) &&
           std::get<1>(tran.first) == std::get<1>(v1) &&
           std::get<2>(tran.first) == std::get<2>(v1)) {
            for(auto& sec : tran.second) {
                result.insert(std::make_tuple(std::get<0>(sec), std::get<1>(sec), std::get<2>(sec), std::get<3>(sec), std::get<4>(sec)));
            }
        }
    }
    return result;
}

/**
 * Transition from m_m_v0F to m_v1F is stored in m_EFA0
 * Transition from m_v1F to m_v0F is stored in m_EFA1
 * @tparam State
 * @tparam Symbol
 * @param transitions
 */
template<typename State, typename Symbol>
void fairParityGame<State, Symbol>::constructEFA(std::map<std::pair<State, Symbol>, std::set<State>>& transitions) {
    using namespace std;

    for(const auto &v0 : m_v0F) {
        for(const auto &v1 : m_v1F) {
            if(get<0>(v0) == get<0>(v1)) {
                if(this->isTransition(get<1>(v0), get<2>(v0), get<1>(v1), transitions)) {
                    //m_EFA0[make_pair(get<1>(v0), get<2>(v0))].insert(get<1>(v1));
                    m_EFA0[v0].insert(v1);
                }
            }
            if(get<1>(v0) == get<1>(v1)) {
                if(this->isTransition(get<0>(v1), get<2>(v0), get<0>(v0), transitions)) {
                    //m_EFA1[make_pair(get<0>(v1), get<2>(v0))].insert(get<0>(v0));
                    m_EFA1[v1].insert(v0);
                }
            }
        }
    }
}

/**
 * Returns a priority to a state for Player 1
 * @tparam State
 * @tparam Symbol
 * @param acceptingState
 * @param m_v1F
 * @param v1s
 * @return
 */
template<typename State, typename Symbol>
int fairParityGame<State, Symbol>::getPrioritym_v1F(std::set<State> &acceptingState, State m_v1First, State v1second) {
    using namespace std;

    for(const auto& accSt : acceptingState) {
        if(v1second == accSt) {
            return 0;
        } else if(acceptingState.find(m_v1First) != acceptingState.end() && acceptingState.find(v1second) == acceptingState.end()) {
            return 1;
        }
    }

    return 2;
}

/**
 * Returns a priority for Player 0's state
 * @tparam State
 * @tparam Symbol
 * @param acceptingState
 * @param v0
 * @return
 */
template<typename State, typename Symbol>
int fairParityGame<State, Symbol>::getPrioritym_v0F(std::set<State> &acceptingState, State v0Second) {
    using namespace std;

    for(const auto& accSt : acceptingState) {
        if(v0Second == accSt) return 0;
    }

    return 2;
}

/**
 * Create all the states that are owned by Player 0
 * @tparam State
 * @tparam Symbol
 * @param a
 */
template <typename State, typename Symbol>
void fairParityGame<State, Symbol>::constructm_v0F(Automaton<State, Symbol> a) {
    using namespace std;
    auto transition = a.getTransitions();
    auto states = a.getStates();
    auto alphabet = a.getAlphabet();

    for(const auto& st : states) {
        for(const auto& v1 : m_v1F) {
            for(const auto& a : alphabet) {
                for(const auto& [p, val] : transition) {
                    for(const auto& setval : val) {
                        if((st == p.first && a == p.second && get<0>(v1) == setval)) {
                            m_v0F.insert(make_tuple(get<0>(v1), get<1>(v1), a, -1, 0));
                        }
                    }
                }
            }
        }
    }

    return;
}

/**
 * Create all the states that are owned by Player 1
 * @tparam State
 * @tparam Symbol
 * @param states
 */
template <typename State, typename Symbol>
void fairParityGame<State, Symbol>::constructm_v1F(std::set<State> &states) {
    using namespace std;

    for(auto &x : states) {
        for(auto &y : states) {
            m_v1F.insert({make_tuple(x, y, -1, 0)});
        }
    }

    return;
}

/**
 * Main function for the parity game construction
 * Create all of the parts needed
 * At this part we receive a parity game graph
 * @tparam State
 * @tparam Symbol
 * @param omega
 * @param transitions
 */
template <typename State, typename Symbol>
void fairParityGame<State, Symbol>::constructFPG(Automaton<State, Symbol> &omega, std::map<std::pair<State, Symbol>, std::set<std::string>> &transitions) {
    using namespace std;

    // construct m_v1F accordingly
    set<State> states = omega.getStates();
    set<State> acceptingStates = omega.getAcceptingStates();

    constructm_v1F(states);

    // construct m_v0F
    constructm_v0F(omega);

    for(const auto& v0 : m_v0F) {
        int p = getPrioritym_v0F(acceptingStates, get<1>(v0));
        m_v0F.erase(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), -1, 0));
        m_v0F.insert(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), p, 0));
    }

    for(const auto& v1 : m_v1F) {
        int p = getPrioritym_v1F(acceptingStates, get<0>(v1), get<1>(v1));
        m_v1F.erase(make_tuple(get<0>(v1), get<1>(v1), -1, 0));
        m_v1F.insert(make_tuple(get<0>(v1), get<1>(v1), p, 0));
    }

    constructEFA(transitions);

#ifdef _DEBUG

   cout << "m_v0F: \n";
   for (const auto& x : m_v0F) {
       cout << get<0>(x) << ' ' << get<1>(x) << ' ' << get<2>(x) << ' ' << get<3>(x) << ' ' << get<4>(x)<< '\n';
   }

   cout << endl;

   cout << "m_v1F: \n";
   for(const auto &x : m_v1F) {
       cout << get<0>(x) << ' ' << get<1>(x) << ' ' << get<2>(x) << ' ' << get<3>(x) << '\n';
   }

   cout << "EFA0" << "\n";
   for(const auto &elem : m_EFA0) {
       for(const auto & e : elem.second) {
           cout << "(" << get<0>(elem.first) << ", " << get<1>(elem.first) << ", " << get<2>(elem.first) << ", " << get<3>(elem.first) << ", " << get<4>(elem.first) << ") ---> ";
           cout << "(" << get<0>(e) << ", " << get<1>(e) << ", " << get<2>(e) << ", " << get<3>(e) << ")\n";
       }
   }

   cout << "EFA1" << "\n";
   for(const auto &elem : m_EFA1) {
       for(const auto & e : elem.second) {
           cout << "(" << get<0>(elem.first) << ", " << get<1>(elem.first) << ", " << get<2>(elem.first) << ", " << get<3>(elem.first) << ") ---> ";
           cout << "(" << get<0>(e) << ", " << get<1>(e) << ", " << get<2>(e) << ", " << get<3>(e) << ", " << get<4>(e) << ")\n";
       }
   }

#endif
    std::cout << "Constructed the fair parity game...\n\n";
    return;
}

#endif