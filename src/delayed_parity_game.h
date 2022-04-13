#ifndef DELAYED_PARITY_GAME_H
#define DELAYED_PARITY_GAME_H

// template<typename State, typename Symbol> using Vertex0 = std::vector<State, State, Symbol>;
// template<typename State> using Vertex1 = std::vector<State, State>;

template<typename State, typename Symbol>
class delayedParityGame : protected Automaton<State, Symbol> {
    public:
        struct DelayedV0 {
            uint id;
            bool bit;
            State s1;
            State s2;
            Symbol symbol;
            uint priority;
            int rho;

            DelayedV0& operator =(const DelayedV0& a) {
                id = a.id;
                bit = a.bit;
                s1 = a.s1;
                s2 = a.s2;
                symbol = a.symbol;
                priority = a.priority;
                rho = a.rho;

                return *this;
            }

            friend bool operator<(const DelayedV0& lhs, const DelayedV0& rhs) {
                return lhs.id < rhs.id;
            }
        };
    public:
        struct DelayedV1 {
            uint id;
            bool bit;
            State s1;
            State s2;
            int priority;
            int rho;

            DelayedV1& operator =(const DelayedV1& a) {
                id = a.id;
                bit = a.bit;
                s1 = a.s1;
                s2 = a.s2;
                priority = a.priority;
                rho = a.rho;

                return *this;
            }

            friend bool operator<(const DelayedV1& lhs, const DelayedV1& rhs) {
                return lhs.id < rhs.id;
            }
        };
    protected:
        std::set<DelayedV0> dv0Set;
        std::set<DelayedV1> dv1Set;
        std::map<DelayedV0, std::set<DelayedV1>> EDA0;
        std::map<DelayedV1, std::set<DelayedV0>> EDA1;
    public:
        delayedParityGame() = default;
        std::set<DelayedV1> &get_dv1Set() { return this->dv1Set; }
        std::set<DelayedV0> &get_dv0Set() { return this->dv0Set; }
        std::map<DelayedV0, std::set<DelayedV1>> &get_EDA0() { return this->EDA0; }
        std::map<DelayedV1, std::set<DelayedV0>> &get_EDA1() { return this->EDA1; }

        void construct_dv1(Automaton<State, Symbol> &omega);
        void construct_dv0(Automaton<State, Symbol> &omega);
        void constructDPG(Automaton<State, Symbol> &omega);
        void construct_EDA(Automaton<State, Symbol> &omega);
        //void construct_EDA1(Automaton<State, Symbol> &omega);

        std::set<DelayedV1> succZero(typename delayedParityGame<State,Symbol>::DelayedV0 v0);
        std::set<DelayedV0> succOne(typename delayedParityGame<State,Symbol>::DelayedV1 v1);
};

template<typename State, typename Symbol>
std::set<typename delayedParityGame<State,Symbol>::DelayedV1> delayedParityGame<State, Symbol>::succZero(typename delayedParityGame<State,Symbol>::DelayedV0 v0) {
    std::set<DelayedV1> result;
    DelayedV1 tmp_v1 = DelayedV1();

    for(const auto &tran : EDA0) {
        if(tran.first.s1 == v0.s1 &&
           tran.first.s2 == v0.s2 &&
           tran.first.symbol == v0.symbol &&
           tran.first.priority == v0.priority) {
            for(const auto &sec : tran.second) {
                tmp_v1 = {
                        sec.id, // id to sort the set by
                        sec.bit, // bit
                        sec.s1, // state from
                        sec.s2, // state to
                        sec.priority, // priority
                        sec.rho // rho
                };
                result.insert({tmp_v1});
            }
        }
    }
    return result;
}

template<typename State, typename Symbol>
std::set<typename delayedParityGame<State,Symbol>::DelayedV0> delayedParityGame<State, Symbol>::succOne(typename delayedParityGame<State,Symbol>::DelayedV1 v1) {
    std::set<DelayedV0> result;
    DelayedV0 tmp_v0 = DelayedV0();

    for(const auto &tran : EDA1) {
        if(tran.first.s1 == v1.s1 &&
           tran.first.s2 == v1.s2 &&
           tran.first.priority == v1.priority) {
            for(const auto &sec : tran.second) {
                tmp_v0 = {
                        sec.id, // id to sort the set by
                        sec.bit, // bit
                        sec.s1, // state from
                        sec.s2, // state to
                        sec.symbol, // symbol
                        sec.priority, // priority
                        sec.rho // rho
                };
                result.insert({tmp_v0});
            }
        }
    }

    return result;
}

template<typename State, typename Symbol>
void delayedParityGame<State, Symbol>::construct_EDA(Automaton<State, Symbol> &omega) {
    auto transitions = omega.getTransitions();
    for(const auto &dv1 : dv1Set) {
        for(const auto &dv0 : dv0Set) {
            if(dv1.s1 == dv0.s1) { // spoiler's moves
                if(dv1.bit == 0) {
                    if(omega.isAcceptingState(dv1.s2) && omega.isTransition(dv0.s2, dv0.symbol, dv1.s2, transitions)) {
                        EDA0[dv0].insert(dv1);
                    }
                } else if(dv0.bit == dv1.bit) {
                    if(!omega.isAcceptingState(dv1.s2) && omega.isTransition(dv0.s2, dv0.symbol, dv1.s2, transitions)) {
                        EDA0[dv0].insert(dv1);
                    }
                }
            } if(dv1.s2 == dv0.s2) { // duplicator's moves
                if(dv0.bit == 1) {
                    if(omega.isAcceptingState(dv0.s1) && omega.isTransition(dv1.s1, dv0.symbol, dv0.s1, transitions)) {
                        EDA1[dv1].insert(dv0);
                    }
                } else if(dv0.bit == dv1.bit) {
                    if(!omega.isAcceptingState(dv0.s1) && omega.isTransition(dv1.s1, dv0.symbol, dv0.s1, transitions)) {
                        EDA1[dv1].insert(dv0);
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

    uint idCnt = 0;
    DelayedV0 tmp_v0 = DelayedV0();
    bool bit = false;

    for(const auto &st1 : states) {
        for(const auto &st2 : states) {
            for(const auto &a : alphabet) {
                if(omega.isTransition(st2, a, st1, transitions)) {
                    for(int i = 0; i < 2; i++) {
                        tmp_v0 = {
                                idCnt, // id to sort the set by
                                bool(i), // bit
                                st1, // state from
                                st2, // state to
                                a, // alpha
                                2, // priority
                                0 // rho
                        };
                        dv0Set.insert({tmp_v0});
                        idCnt++;
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
    DelayedV1 tmp_v1 = DelayedV1();
    uint idCnt = 0;

    for(const auto &st1 : states) {
        for(const auto &st2 : states) {
            for(int i = 0; i < 2; i++) {
                if(omega.isAcceptingState(st2)) {
                    tmp_v1 = {
                            idCnt, // id to sort the set by
                            0, // bit
                            st1, // state from
                            st2, // state to
                            0, // priority
                            0 // rho
                    };
                    dv1Set.insert({tmp_v1});
                    idCnt++;
                    break;
                } else {
                    tmp_v1 = {
                            idCnt, // id to sort the set by
                            bool(i), // bit
                            st1, // state from
                            st2, // state to
                            bool(i), // priority
                            0 // rho
                    };
                    dv1Set.insert({tmp_v1});
                    idCnt++;
                }
            }
        }
    }

    return;
}

template<typename State, typename Symbol>
void delayedParityGame<State, Symbol>::constructDPG(Automaton<State, Symbol> &omega) {
    auto states = omega.getStates();
    construct_dv1(omega);

    construct_dv0(omega);

    construct_EDA(omega);

    #ifdef _DEBUG

    std::cout << "Delayed V1: \n";
    for(const auto &vertex : dv1Set) {
        std::cout << "(" << vertex.bit << ", " << vertex.s1 << ", " << vertex.s2 << ", " << vertex.priority << ", " << vertex.rho << ")" << std::endl;
    }

    std::cout << "Delayed V0: \n";
    for(const auto &vertex : dv0Set) {
        std::cout << "(" << vertex.bit << ", " << vertex.s1 << ", " << vertex.s2 << ", " << vertex.symbol << ", " << vertex.priority << ", " << vertex.rho << ")" << std::endl;
    }

    std::cout << "Delayed transitions E0: \n";
    for(const auto &vertex : EDA0) {
        for(const auto &sec : vertex.second) {
            std::cout << "(" << vertex.first.bit << ", " << vertex.first.s1 << ", " << vertex.first.s2
            << ", " << vertex.first.symbol << ") ---> (" << sec.bit << ", " << sec.s1 << ", " <<
             sec.s2 << ")" << std::endl;
        }
    }

    std::cout << "Delayed transitions E1: \n";
    for(const auto &vertex : EDA1) {
        for(const auto &sec : vertex.second) {
            std::cout << "(" << vertex.first.bit << ", " << vertex.first.s1 << ", " << vertex.first.s2
                      << ") ---> (" << sec.bit << ", " << sec.s1 << ", " <<
                      sec.s2 << ", " << sec.symbol << ")" << std::endl;
        }
    }

    #endif

    return;
}

#endif