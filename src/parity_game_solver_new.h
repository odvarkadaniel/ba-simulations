#ifndef PARITY_GAME_SOLVER_NEW_H
#define PARITY_GAME_SOLVER_NEW_H

#include"delayed_parity_game.h"

#define INF 10

template<class Game, typename State, typename Symbol>
class pgSolver : protected delayedParityGame<State, Symbol> {
    public:
        void solvePG(Game pg, Automaton<State, Symbol> &omega);
        int updateZero(typename delayedParityGame<State,Symbol>::DelayedV0 dv0, Game pg, unsigned int n1);
        int updateOne(typename delayedParityGame<State,Symbol>::DelayedV1 dv1, Game pg, unsigned int n1);

        unsigned int n_1(Game pg) const;
};

template<typename Game, typename State, typename Symbol>
void pgSolver<Game, State, Symbol>::solvePG(Game pg, Automaton<State, Symbol> &omega) {
    //std::cout << "n1 = " << n_1(pg) << "\n";

    auto v0Set = pg.get_dv0Set();
    auto v1Set = pg.get_dv1Set();
    auto E0 = pg.get_EDA0();
    auto E1 = pg.get_EDA1();

    std::vector<typename delayedParityGame<State,Symbol>::DelayedV0> temp_v0;
    std::vector<typename delayedParityGame<State,Symbol>::DelayedV1> temp_v1;
    typename delayedParityGame<State,Symbol>::DelayedV0 changev0 = typename delayedParityGame<State,Symbol>::DelayedV0();
    typename delayedParityGame<State,Symbol>::DelayedV1 changev1 = typename delayedParityGame<State,Symbol>::DelayedV1();

    const unsigned int n1 = n_1(pg);
    if(!n1) std::cerr << "There is no vertex of the parity game with priority 0.\n";

    bool changed = true;
    while(changed) {
        changed = false;
        for(auto &v0 : v0Set) {
            int updatedV = updateZero(v0, pg, n1);
            if(updatedV != v0.rho) {
                changev0 = { v0.id, v0.bit, v0.s1, v0.s2, v0.symbol, v0.priority, updatedV };
                temp_v0.push_back({changev0});
                changed = true;
            }
        }

        for(const auto &insert : temp_v0) {
            v0Set.erase(insert);
            v0Set.insert(insert);
        }
        temp_v0.clear();

        for(auto &v1 : v1Set) {
            int updatedV = updateOne(v1, pg, n1);
            if(updatedV != v1.rho) {
                changev1 = { v1.id, v1.bit, v1.s1, v1.s2, v1.priority, updatedV };
                temp_v1.push_back({changev1});
                changed = true;
            }
        }

        for(const auto &insert : temp_v1) {
            v1Set.erase(insert);
            v1Set.insert(insert);
        }
        temp_v1.clear();
    }

    // print the result of the game
    std::cout << "Final result of the delayed simulation: \n";
    for(const auto &v1 : v1Set) { std::cout << "(" << v1.bit << ", " << v1.s1 << ", " << v1.s2 << ", " << v1.rho << ")\n"; }

    // Get the relation of the simulation
    for(const auto &v1 : v1Set) {
        if(v1.rho < INF) {
            if(v1.bit == 1 && omega.isAcceptingState(v1.s1) && !omega.isAcceptingState(v1.s2)) {
                std::cout << v1.s1 << " delayed simulates " << v1.s2 << "\n";
            } else if(v1.bit == 0 && !omega.isAcceptingState(v1.s1) && omega.isAcceptingState(v1.s2)) {
                std::cout << v1.s1 << " delayed simulates " << v1.s2 << "\n";
            }
        }
    }

    return;
}

template<typename Game, typename State, typename Symbol>
int pgSolver<Game, State, Symbol>::updateZero(typename delayedParityGame<State,Symbol>::DelayedV0 v0, Game pg, unsigned int n1) {
    if(v0.priority == 0) {
        auto successors = pg.succZero(v0);
        if(successors.size() == 0) {
            for(auto &itr : this->EDA1) {
                for(auto &sec : itr.second) {
                    if(sec.bit == v0.bit &&
                       sec.s1 == v0.s1 &&
                       sec.s2 == v0.s2 &&
                       sec.symbol == v0.symbol &&
                       sec.priority == v0.priority) {
                        this->EDA1[itr.first].erase(v0);
                        v0.rho = INF;
                        this->EDA1[itr.first].insert(v0);
                    }
                }
            }
            return INF;
        }
        return 0;
    } else if(v0.priority == 2) {
        auto successors = pg.succZero(v0);
        if(successors.size() == 0) {
            for(auto &itr : this->EDA1) {
                for(auto &sec : itr.second) {
                    if(sec.bit == v0.bit &&
                       sec.s1 == v0.s1 &&
                       sec.s2 == v0.s2 &&
                       sec.symbol == v0.symbol &&
                       sec.priority == v0.priority) {
                        this->EDA1[itr.first].erase(v0);
                        v0.rho = INF;
                        this->EDA1[itr.first].insert(v0);
                    }
                }
            }
            return INF;
        }
        return v0.rho;
    } else { // p(v) = 1
        auto successors = pg.succZero(v0);
        if(successors.size() == 0) return INF;

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(succ.rho);
        }

        auto maxRho = *min_element(begin(rhos), end(rhos));

        if(maxRho == INF || maxRho + 1 > n1) return INF;

        return (maxRho + 1);
    }
}

template<typename Game, typename State, typename Symbol>
int pgSolver<Game, State, Symbol>::updateOne(typename delayedParityGame<State,Symbol>::DelayedV1 v1, Game pg, unsigned int n1) {
    if(v1.priority == 0) {
        auto successors = pg.succOne(v1);
        if(successors.size() == 0) return INF;

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(succ.rho);
        }
        auto maxRho = *max_element(begin(rhos), end(rhos));

        if(maxRho == INF) return INF;

        return 0;
    } else if(v1.priority == 2) {
        auto successors = pg.succOne(v1);
        if(successors.size() == 0) return INF;

        return v1.rho;
    } else { // p(v) = 1
        auto successors = pg.succOne(v1);
        if(successors.size() == 0) return INF;

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(succ.rho);
        }

        auto maxRho = *max_element(begin(rhos), end(rhos));

        if(maxRho == INF || maxRho + 1 > n1) return INF;

        return (maxRho + 1);
    }
}

template<typename Game, typename State, typename Symbol>
unsigned int pgSolver<Game, State, Symbol>::n_1(Game pg) const {
    unsigned int counter = 0;

    for(const auto &v1 : pg.get_dv1Set()) {
        if(v1.priority == 1) counter++;
    }
    for(const auto &v0 : pg.get_dv0Set()) {
        if(v0.priority == 1) counter++;
    }

    return counter;
}

#endif
