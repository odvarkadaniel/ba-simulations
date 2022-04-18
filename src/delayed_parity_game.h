#ifndef DELAYED_PARITY_GAME_H
#define DELAYED_PARITY_GAME_H

#include"automaton.h"

template<typename State, typename Symbol>
class delayedParityGame : protected Automaton<State, Symbol> {
protected:
    std::set<std::tuple<bool, State, State, Symbol, int, int>> m_v0D; // set with Player 0's states
    std::set<std::tuple<bool, State, State, int, int>> m_v1D; // set with Player 1's states
    std::map<std::tuple<bool, State, State, Symbol, int, int>, std::set<std::tuple<bool, State, State, int, int>>> m_EDA0;
    std::map<std::tuple<bool, State, State, int, int>, std::set<std::tuple<bool, State, State, Symbol, int, int>>> m_EDA1;
public:
    std::set<std::tuple<bool, State, State, Symbol, int, int>> getv0D() { return this->m_v0D; };
    std::set<std::tuple<bool, State, State, int, int>> getv1D() { return this->m_v1D; };
    std::map<std::tuple<bool, State, State, Symbol, int, int>, std::set<std::tuple<bool, State, State, int, int>>> &getEDA0() { return this->m_EDA0; };
    std::map<std::tuple<bool, State, State, int, int>, std::set<std::tuple<bool, State, State, Symbol, int, int>>> &getEDA1() { return this->m_EDA1; };

    void construct_dv1(Automaton <State, Symbol> &omega);
    void construct_dv0(Automaton <State, Symbol> &omega);
    void constructDPG(Automaton <State, Symbol> &omega);
    void construct_EDA(Automaton <State, Symbol> &omega);

    std::set<std::tuple<bool, State, State, int, int>> succ0(std::tuple<bool, State, State, Symbol, int, int> &v0, std::map<std::tuple<bool, State, State, Symbol, int, int>, std::set<std::tuple<bool, State, State, int, int>>> &EDA0);
    std::set<std::tuple<bool, State, State, Symbol, int, int>> succ1(std::tuple<bool, State, State, int, int> &v1, std::map<std::tuple<bool, State, State, int, int>, std::set<std::tuple<bool, State, State, Symbol, int, int>>> &EDA1);
};

template<typename State, typename Symbol>
std::set<std::tuple<bool, State, State, int, int>> delayedParityGame<State, Symbol>::succ0(std::tuple<bool, State, State, Symbol, int, int> &v0, std::map<std::tuple<bool, State, State, Symbol, int, int>, std::set<std::tuple<bool, State, State, int, int>>> &EDA0) {
    //std::cout << "(" << std::get<0>(v0) << ", " << std::get<1>(v0) << ", " << std::get<2>(v0) << ")\n";
    //auto EFA0 = getEFA0();

    std::set<std::tuple<bool, State, State, int, int>> result;
    for(auto &tran : EDA0) {
        //std::cout << "(" << std::get<0>(tran.first) << ", " << std::get<1>(tran.first) << ", " << std::get<2>(tran.first) << ")\n";
        if(std::get<0>(tran.first) == std::get<0>(v0) &&
           std::get<1>(tran.first) == std::get<1>(v0) &&
           std::get<2>(tran.first) == std::get<2>(v0) &&
           std::get<3>(tran.first) == std::get<3>(v0) &&
           std::get<4>(tran.first) == std::get<4>(v0)) {
            for(auto& sec : tran.second) {
                result.insert(std::make_tuple(std::get<0>(sec), std::get<1>(sec), std::get<2>(sec), std::get<3>(sec), std::get<4>(sec)));
            }
        }
    }
    return result;
}

template<typename State, typename Symbol>
std::set<std::tuple<bool, State, State, Symbol, int, int>> delayedParityGame<State, Symbol>::succ1(std::tuple<bool, State, State, int, int> &v1, std::map<std::tuple<bool, State, State, int, int>, std::set<std::tuple<bool, State, State, Symbol, int, int>>> &EDA1) {
    //std::cout << "(" << std::get<0>(v1) << ", " << std::get<1>(v1) << ", " << std::get<2>(v1) << ")\n";
    //auto EFA1 = getEFA1();
    std::set<std::tuple<bool, State, State, Symbol, int, int>> result;
    for(auto &tran : EDA1) {
        //std::cout << "(" << std::get<0>(tran.first) << ", " << std::get<1>(tran.first) << ", " << std::get<2>(tran.first) << ")\n";
        if(std::get<0>(tran.first) == std::get<0>(v1) &&
           std::get<1>(tran.first) == std::get<1>(v1) &&
           std::get<2>(tran.first) == std::get<2>(v1) &&
           std::get<3>(tran.first) == std::get<3>(v1)) {
            for(auto& sec : tran.second) {
                result.insert(std::make_tuple(std::get<0>(sec), std::get<1>(sec), std::get<2>(sec), std::get<3>(sec), std::get<4>(sec), std::get<5>(sec)));
            }
        }
    }
    return result;
}

template<typename State, typename Symbol>
void delayedParityGame<State, Symbol>::construct_EDA(Automaton<State, Symbol> &omega) {
    using namespace std;

    auto transitions = omega.getTransitions();
    for(const auto &dv1 : m_v1D) {
        for(const auto &dv0 : m_v0D) {
            if(get<1>(dv1) == get<1>(dv0)) { // spoiler's moves
                if(get<0>(dv0) == get<0>(dv1)) {
                    if(!omega.isAcceptingState(get<2>(dv1)) && omega.isTransition(get<2>(dv0), get<3>(dv0), get<2>(dv1), transitions)) {
                        m_EDA0[dv0].insert(dv1);
                    }
                }
                if(get<0>(dv1) == 0) {
                    if(omega.isAcceptingState(get<2>(dv1)) && omega.isTransition(get<2>(dv0), get<3>(dv0), get<2>(dv1), transitions)) {
                        m_EDA0[dv0].insert(dv1);
                    }
                }
            } if(get<2>(dv1) == get<2>(dv0)) { // duplicator's moves
                if(get<0>(dv0) == get<0>(dv1)) {
                    if(!omega.isAcceptingState(get<1>(dv0)) && omega.isTransition(get<1>(dv1), get<3>(dv0), get<1>(dv0), transitions)) {
                        m_EDA1[dv1].insert(dv0);
                    }
                }
                if(get<0>(dv0)) {
                    if(omega.isAcceptingState(get<1>(dv0)) && omega.isTransition(get<1>(dv1), get<3>(dv0), get<1>(dv0), transitions)) {
                        m_EDA1[dv1].insert(dv0);
                    }
                }
            }
        }
    }

    return;
}

template<typename State, typename Symbol>
void delayedParityGame<State, Symbol>::construct_dv0(Automaton<State, Symbol> &omega) {
    auto states = omega.getStates();
    auto alphabet = omega.getAlphabet();
    auto transitions = omega.getTransitions();

    for(const auto &st1 : states) {
        for(const auto &st2 : states) {
            for(const auto &a : alphabet) {
                if(omega.existsTransition(st1, a)) {
                    for(int i = 0; i < 2; i++) {
                        m_v0D.insert(std::make_tuple(bool(i), st1, st2, a, 2, 0));
                    }
                }
            }
        }
    }
}

template<typename State, typename Symbol>
void delayedParityGame<State, Symbol>::construct_dv1(Automaton<State, Symbol> &omega) {
    auto states = omega.getStates();
    bool bit = false;

    for(const auto &st1 : states) {
        for(const auto &st2 : states) {
            for(int i = 0; i < 2; i++) {
                if(omega.isAcceptingState(st2)) {
                    m_v1D.insert(std::make_tuple(0, st1, st2, 0, 0));
                    break;
                } else {
                    m_v1D.insert(std::make_tuple(bool(i), st1, st2, i, 0));
                }
            }
        }
    }

    return;
}

template<typename State, typename Symbol>
void delayedParityGame<State, Symbol>::constructDPG(Automaton<State, Symbol> &omega) {
    using namespace std;

    construct_dv1(omega);
    construct_dv0(omega);
    construct_EDA(omega);

#ifdef _DEBUG

    std::cout << "Delayed V1: \n";
    for(const auto &vertex : m_v1D) {
        std::cout << "(" << get<0>(vertex) << ", " << get<1>(vertex) << ", " << get<2>(vertex) << ", " << get<3>(vertex) << ", " << get<4>(vertex) << ")" << std::endl;
    }

    std::cout << "Delayed V0: \n";
    for(const auto &vertex : m_v0D) {
        std::cout << "(" << get<0>(vertex) << ", " << get<1>(vertex) << ", " << get<2>(vertex) << ", " << get<3>(vertex) << ", " << get<4>(vertex) << ", " << get<5>(vertex) << ")" << std::endl;
    }

    std::cout << "Delayed transitions E0: \n";
    for(const auto &vertex : m_EDA0) {
        for(const auto &sec : vertex.second) {
            std::cout << "(" << get<0>(vertex.first) << ", " << get<1>(vertex.first) << ", " << get<2>(vertex.first)
                      << ", " << get<3>(vertex.first) << ") ---> (" << get<0>(sec) << ", " << get<1>(sec) << ", " <<
                      get<2>(sec) << ")" << std::endl;
        }
    }

    std::cout << "Delayed transitions E1: \n";
    for(const auto &vertex : m_EDA1) {
        for(const auto &sec : vertex.second) {
            std::cout << "(" << get<0>(vertex.first) << ", " << get<1>(vertex.first) << ", " << get<2>(vertex.first)
                      << ") ---> (" << get<0>(sec) << ", " << get<1>(sec) << ", " <<
                      get<2>(sec) << ", " << get<3>(sec) << ")" << std::endl;
        }
    }

#endif
    std::cout << "Constructed the delayed parity game...\n\n";
    return;
}

#endif
