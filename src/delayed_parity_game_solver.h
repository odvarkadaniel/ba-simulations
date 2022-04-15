#ifndef DELAYED_PARITY_GAME_SOLVER_H
#define DELAYED_PARITY_GAME_SOLVER_H

#include<iostream>
#include<algorithm>
#include"delayed_parity_game.h"

#define UPDATE_EDA1(x) \
do {  \
    for(auto &itr : EDA1) { \
        for(auto &sec : itr.second) { \
            if(std::get<0>(sec) == std::get<0>(v0) && \
                std::get<1>(sec) == std::get<1>(v0) && \
                std::get<2>(sec) == std::get<2>(v0) && \
                std::get<3>(sec) == std::get<3>(v0) && \
                std::get<4>(sec) == std::get<4>(v0)) { \
                    EDA1[itr.first].erase(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<4>(v0), get<5>(v0))); \
                    EDA1[itr.first].insert(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<4>(v0), x)); \
            } \
        } \
    } \
} while(0)

#define UPDATE_EDA0(x) \
do {                   \
    for(auto &itr : EDA0) { \
        for(auto &sec : itr.second) { \
            if(std::get<0>(sec) == std::get<0>(v1) && \
                std::get<1>(sec) == std::get<1>(v1) && \
                std::get<2>(sec) == std::get<2>(v1) && \
                std::get<3>(sec) == std::get<3>(v1)) { \
                    EDA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1), get<4>(v1))); \
                    EDA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1), x)); \
            } \
        } \
    } \
} while(0)

#define INF 10

template<typename State, typename Symbol> using dVertex0 = std::set<std::tuple<bool, State, State, Symbol, int, int>>;
template<typename State> using dVertex1 = std::set<std::tuple<bool, State, State, int, int>>;

template<class Game, typename State, typename Symbol>
class delayedParityGameSolver : public delayedParityGame<State, Symbol> {
public:
    std::set<std::pair<State, State>> solveParityGame(Game pg, Automaton<State, Symbol> &omega);
    int updateD0(std::tuple<bool, State, State, Symbol, int, int> v0, Game pg, uint n1, std::map<std::tuple<bool, State, State, int, int>, std::set<std::tuple<bool, State, State, Symbol, int, int>>> &EDA1, std::map<std::tuple<bool, State, State, Symbol, int, int>, std::set<std::tuple<bool, State, State, int, int>>> &EDA0);
    int updateD1(std::tuple<bool, State, State, int, int> v1, Game pg, uint n1, std::map<std::tuple<bool, State, State, Symbol, int, int>, std::set<std::tuple<bool, State, State, int, int>>> &EDA0, std::map<std::tuple<bool, State, State, int, int>, std::set<std::tuple<bool, State, State, Symbol, int, int>>> &EDA1);
    uint n_1(dVertex0<State, Symbol> &v0F, dVertex1<State> &v1F);
};

template<typename Game, typename State, typename Symbol>
std::set<std::pair<State, State>> delayedParityGameSolver<Game, State, Symbol>::solveParityGame(Game pg, Automaton<State, Symbol> &omega) {
    using namespace std;

    auto v0D = pg.getv0D();
    auto v1D = pg.getv1D();
    auto EDA0 = pg.getEDA0();
    auto EDA1 = pg.getEDA1();

    uint const vertex0Length = v0D.size();
    uint const vertex1Length = v1D.size();

    const uint n1 = n_1(v0D, v1D);
//    if(!n1) {
//        std::cout << "There is no vertex with priority 1\n";
//    }

    std::vector<std::tuple<bool, State, State, Symbol, int, int, int>> temp_v0;
    std::vector<std::tuple<bool, State, State, int, int, int>> temp_v1;

    bool changed = true;
    while(changed) {
        changed = false;
        for(auto &v0 : v0D) {
            int updatedV = updateD0(v0, pg, n1, EDA1, EDA0);
            if(updatedV != std::get<5>(v0)) {
                temp_v0.push_back(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<4>(v0), get<5>(v0), updatedV));
                changed = true;
            }
        }

        for(const auto &insert : temp_v0) {
            v0D.erase(make_tuple(get<0>(insert), get<1>(insert), get<2>(insert), get<3>(insert), get<4>(insert), get<5>(insert)));
            v0D.insert(make_tuple(get<0>(insert), get<1>(insert), get<2>(insert), get<3>(insert), get<4>(insert), get<6>(insert)));
        }
        temp_v0.clear();

        for(auto &v1 : v1D) {
            int updatedV = updateD1(v1, pg, n1, EDA0, EDA1);
            if(updatedV != std::get<4>(v1)) {
                temp_v1.push_back(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1), get<4>(v1), updatedV));
                changed = true;
            }
        }

        for(const auto &insert : temp_v1) {
            v1D.erase(make_tuple(get<0>(insert), get<1>(insert), get<2>(insert), get<3>(insert), get<4>(insert)));
            v1D.insert(make_tuple(get<0>(insert), get<1>(insert), get<2>(insert), get<3>(insert), get<5>(insert)));
        }
        temp_v1.clear();
    }
//    std::cout << "\n\n";
//    for(const auto &x : v0D) { std::cout << "(" << std::get<0>(x) << ", " << std::get<1>(x) << ", " << std::get<2>(x) << ", " << std::get<3>(x) << ", " << std::get<4>(x) << ", " << get<5>(x) << ")\n"; }
//    for(const auto &x : v1D) { std::cout << "(" << std::get<0>(x) << ", " << std::get<1>(x) << ", " << std::get<2>(x) << ", " << std::get<3>(x) << ", " << std::get<4>(x) <<")\n"; }
//
//    std::cout << "Solved the parity game!" << std::endl;

    // Get the relation of the simulation
    std::set<std::pair<State, State>> result;
    for(const auto &v1 : v1D) {
        if(get<4>(v1) < INF) {
            if (get<0>(v1) == 1 && omega.isAcceptingState(get<1>(v1)) && !omega.isAcceptingState(get<2>(v1))) {
                result.insert(std::make_pair(std::get<1>(v1), std::get<2>(v1)));

            } else if(get<0>(v1) == 0 && !omega.isAcceptingState(get<1>(v1)) && omega.isAcceptingState(get<2>(v1))) {
                result.insert(std::make_pair(std::get<1>(v1), std::get<2>(v1)));
            }
            if(get<1>(v1) == get<2>(v1)) {
                result.insert(std::make_pair(std::get<1>(v1), std::get<2>(v1)));
            }
        }
    }
    std::cout << "\n\nSolved the parity game!" << std::endl;
    return result;
}

template<typename Game, typename State, typename Symbol>
int delayedParityGameSolver<Game, State, Symbol>::updateD1(std::tuple<bool, State, State, int, int> v1,
                                                           Game pg,
                                                           const uint n1,
                                                           std::map<std::tuple<bool, State, State, Symbol, int, int>, std::set<std::tuple<bool, State, State, int, int>>> &EDA0,
                                                           std::map<std::tuple<bool, State, State, int, int>, std::set<std::tuple<bool, State, State, Symbol, int, int>>> &EDA1) {
    using namespace std;

    if(std::get<3>(v1) == 0) {
        auto successors = pg.succ1(v1, EDA1);
        if(successors.size() == 0) {
            UPDATE_EDA0(INF);
            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<5>(succ));
        }
        auto maxRho = *max_element(begin(rhos), end(rhos));

        if(maxRho == INF) {
            UPDATE_EDA0(INF);
            return INF;
        }

        if(maxRho != get<4>(v1)) {
            UPDATE_EDA0(maxRho);
        }
        return 0;
    } else { // p(v) is 1
        auto successors = pg.succ1(v1, EDA1);
        if(successors.size() == 0) {
            UPDATE_EDA0(INF);
            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<5>(succ));
        }
        auto maxRho = *max_element(begin(rhos), end(rhos));

        if(maxRho == INF || maxRho + 1 > n1) {
            UPDATE_EDA0(INF);
            return INF;
        }

        if(maxRho+1 != get<4>(v1)) {
            UPDATE_EDA0((maxRho + 1));
        }
        return (maxRho + 1);
    }
}

template<typename Game, typename State, typename Symbol>
int delayedParityGameSolver<Game, State, Symbol>::updateD0(std::tuple<bool, State, State, Symbol, int, int> v0,
                                                           Game pg,
                                                           const uint n1,
                                                           std::map<std::tuple<bool, State, State, int, int>, std::set<std::tuple<bool, State, State, Symbol, int, int>>> &EDA1,
                                                           std::map<std::tuple<bool, State, State, Symbol, int, int>, std::set<std::tuple<bool, State, State, int, int>>> &EDA0) {
    using namespace std;

    auto successors = pg.succ0(v0, EDA0);
    if(successors.size() == 0) {
        UPDATE_EDA1(INF);
        return INF;
    }
    std::vector<int> rhos;
    for(auto &succ : successors) {
        rhos.push_back(get<4>(succ));
    }
    auto maxRho = *min_element(begin(rhos), end(rhos));

    UPDATE_EDA1(maxRho);
    return maxRho;
}

template<typename Game, typename State, typename Symbol>
uint delayedParityGameSolver<Game, State, Symbol>::n_1(dVertex0<State, Symbol> &v0F, dVertex1<State> &v1F) {
    uint counter = 0;

    for(const auto &x : v0F) {
        if(std::get<4>(x) == 1) counter++;
    }
    for(const auto &x : v1F) {
        if(std::get<3>(x) == 1) counter++;
    }

    return counter;
}

#endif
