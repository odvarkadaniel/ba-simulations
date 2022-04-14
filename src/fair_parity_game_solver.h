#ifndef PARITY_GAME_SOLVER_H
#define PARITY_GAME_SOLVER_H

#include"fair_parity_game.h"

#define UPDATE_DFA1(x) \
do {                   \
    for(auto &itr : EFA1) { \
        for(auto &sec : itr.second) { \
            if(std::get<0>(sec) == std::get<0>(v0) && \
                std::get<1>(sec) == std::get<1>(v0) && \
                std::get<2>(sec) == std::get<2>(v0) && \
                std::get<3>(sec) == std::get<3>(v0)) { \
                    EFA1[itr.first].erase(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<4>(v0))); \
                    EFA1[itr.first].insert(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), x)); \
            } \
        } \
    }                       \
} while(0)


#define UPDATE_DFA0(x) \
do {                 \
    for(auto &itr : EFA0) { \
        for(auto &sec : itr.second) { \
            if(std::get<0>(sec) == std::get<0>(v1) && \
                std::get<1>(sec) == std::get<1>(v1) && \
                std::get<2>(sec) == std::get<2>(v1)) { \
                    EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1))); \
                    EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), x)); \
            } \
        } \
    } \
} while(0)

#define INF 10

template<typename State, typename Symbol> using Vertex0 = std::set<std::tuple<State, State, Symbol, int, int>>;
template<typename State> using Vertex1 = std::set<std::tuple<State, State, int, int>>;

template<class Game, typename State, typename Symbol>
class parityGameSolver : public fairParityGame<State, Symbol> {
    public:
        void solveParityGame(Game pg, Automaton<State, Symbol> &omega);
        int update0(std::tuple<State, State, Symbol, int, int> v0, Game pg, uint n1, std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1, std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0);
        int update1(std::tuple<State, State, int, int> v1, Game pg, uint n1, std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0, std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1);
        uint n_1(Vertex0<State, Symbol> &v0F, Vertex1<State> &v1F);
};

template<typename Game, typename State, typename Symbol>
void parityGameSolver<Game, State, Symbol>::solveParityGame(Game pg, Automaton<State, Symbol> &omega) {
    using namespace std;

    auto v0F = pg.getv0F();
    auto v1F = pg.getv1F();
    auto EFA0 = pg.getEFA0();
    auto EFA1 = pg.getEFA1();

    uint const vertex0Length = v0F.size();
    uint const vertex1Length = v1F.size();

    const uint n1 = n_1(v0F, v1F);
    if(!n1) {
        std::cout << "There is no vertex with priority 1\n";
    }

    std::vector<std::tuple<State, State, Symbol, int, int, int>> temp_v0;
    std::vector<std::tuple<State, State, int, int, int>> temp_v1;

    bool changed = true;
    while(changed) {
        changed = false;
        for(auto &v0 : v0F) {
            int updatedV = update0(v0, pg, n1, EFA1, EFA0);
            if(updatedV != std::get<4>(v0)) {
                temp_v0.push_back(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<4>(v0), updatedV));
                changed = true;
            }
        }

        for(const auto &insert : temp_v0) {
            v0F.erase(make_tuple(get<0>(insert), get<1>(insert), get<2>(insert), get<3>(insert), get<4>(insert)));
            v0F.insert(make_tuple(get<0>(insert), get<1>(insert), get<2>(insert), get<3>(insert), get<5>(insert)));
        }
        temp_v0.clear();

        for(auto &v1 : v1F) {
            int updatedV = update1(v1, pg, n1, EFA0, EFA1);
            if(updatedV != std::get<3>(v1)) {
                temp_v1.push_back(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1), updatedV));
                changed = true;
            }
        }

        for(const auto &insert : temp_v1) {
            v1F.erase(make_tuple(get<0>(insert), get<1>(insert), get<2>(insert), get<3>(insert)));
            v1F.insert(make_tuple(get<0>(insert), get<1>(insert), get<2>(insert), get<4>(insert)));
        }
        temp_v1.clear();
    }
    std::cout << "\n\n";
    for(const auto &x : v0F) { std::cout << "(" << std::get<0>(x) << ", " << std::get<1>(x) << ", " << std::get<2>(x) << ", " << std::get<3>(x) << ", " << std::get<4>(x) <<")\n"; }
    for(const auto &x : v1F) { std::cout << "(" << std::get<0>(x) << ", " << std::get<1>(x) << ", " << std::get<2>(x) << ", " << std::get<3>(x) <<")\n"; }
    std::cout << "Solved the parity game!" << std::endl;

    for(const auto &res : v1F) {
        if(std::get<3>(res) < INF)
            std::cout << "(" << std::get<0>(res) << ", " << std::get<1>(res) << ")\n";
    }
}

template<typename Game, typename State, typename Symbol>
int parityGameSolver<Game, State, Symbol>::update1(std::tuple<State, State, int, int> v1,
                                                   Game pg,
                                                   const uint n1,
                                                   std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0,
                                                   std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1) {
    using namespace std;

    if(std::get<2>(v1) == 0) {
        auto successors = pg.succ1(v1, EFA1);
        if(successors.size() == 0) {
            UPDATE_DFA0(INF);
            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<4>(succ));
        }
        auto maxRho = *max_element(begin(rhos), end(rhos));

        if(maxRho == INF) {
            UPDATE_DFA0(INF);
            return INF;
        }

        if(std::get<3>(v1) != maxRho) {
            UPDATE_DFA0(0);
        }

        return 0;
    } else if(std::get<2>(v1) == 2) {
        auto successors = pg.succ1(v1, EFA1);
        if(successors.size() == 0) {
            UPDATE_DFA0(INF);
            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<4>(succ));
        }
        auto maxRho = *min_element(begin(rhos), end(rhos));

        for(auto &itr : EFA0) {
            UPDATE_DFA0(maxRho);
        }
        return maxRho;
    } else { // p(v) is 1
        auto successors = pg.succ1(v1, EFA1);
        if(successors.size() == 0) {
            UPDATE_DFA0(INF);
            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<4>(succ));
        }
        auto maxRho = *max_element(begin(rhos), end(rhos));

        if(maxRho == INF || maxRho + 1 > n1) {
            UPDATE_DFA0(INF);
            return INF;
        }

        if(std::get<3>(v1) != (maxRho + 1)) {
            UPDATE_DFA0((maxRho + 1));
        }

        return (maxRho + 1);
    }
}

template<typename Game, typename State, typename Symbol>
int parityGameSolver<Game, State, Symbol>::update0(std::tuple<State, State, Symbol, int, int> v0, Game pg, const uint n1,
                                                   std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1,
                                                   std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0) {
    using namespace std;

    if(std::get<3>(v0) == 0) {
        auto successors = pg.succ0(v0, EFA0);
        if(successors.size() == 0) {
            UPDATE_DFA1(INF);
            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<3>(succ));
        }
        auto minRho = *min_element(begin(rhos), end(rhos));

        if(minRho == INF) {
            UPDATE_DFA1(INF);
            return INF;
        }

        return 0;
    } else { // p(v) == 2
        auto successors = pg.succ0(v0, EFA0);
        if(successors.size() == 0) {
            UPDATE_DFA1(INF);
            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<3>(succ));
        }
        auto minRho = *min_element(begin(rhos), end(rhos));

        UPDATE_DFA1(minRho);

        return minRho;
    }
}

template<typename Game, typename State, typename Symbol>
uint parityGameSolver<Game, State, Symbol>::n_1(Vertex0<State, Symbol> &v0F, Vertex1<State> &v1F) {
    uint counter = 0;

    for(const auto &x : v0F) {
        if(std::get<3>(x) == 1) counter++;
    }
    for(const auto &x : v1F) {
        if(std::get<2>(x) == 1) counter++;
    }
    
    return counter;
}

#endif