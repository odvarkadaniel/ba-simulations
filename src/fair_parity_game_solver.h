#ifndef PARITY_GAME_SOLVER_H
#define PARITY_GAME_SOLVER_H

#include"fair_parity_game.h"

#define INF 10

template<typename State, typename Symbol> using Vertex0 = std::set<std::tuple<State, State, Symbol, int, int>>;
template<typename State> using Vertex1 = std::set<std::tuple<State, State, int, int>>;

template<class Game, typename State, typename Symbol>
class parityGameSolver : public fairParityGame<State, Symbol> {
    public:
        void solveParityGame(Game pg, Automaton<State, Symbol> &omega);
        int update0(std::tuple<State, State, Symbol, int, int> v0, Game pg, const uint n1, std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1, std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0);
        int update1(std::tuple<State, State, int, int> v1, Game pg, const uint n1, std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0, std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1);
        uint n_1(Vertex0<State, Symbol> &v0F, Vertex1<State> &v1F) const;
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
    //std::cout << vertexLength << std::endl;

    const uint n1 = n_1(v0F, v1F);
    if(!n1) {
        std::cout << "There is no vertex with priority 1\n";
    }
    //std::cout << n1 << std::endl;

    std::vector<std::tuple<State, State, Symbol, int, int, int>> temp_v0;
    std::vector<std::tuple<State, State, int, int, int>> temp_v1;

    bool changed = true;
    while(changed) {
        changed = false;
        for(auto &v0 : v0F) {
            int updatedV = update0(v0, pg, n1, EFA1, EFA0);
            if(updatedV != std::get<4>(v0)) {
                temp_v0.push_back(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<4>(v0), updatedV));
                //for(const auto &x : v0F) { std::cout << "(" << std::get<0>(x) << ", " << std::get<1>(x) << ", " << std::get<2>(x) << ", " << std::get<3>(x) << ", " << std::get<4>(x) <<")\n"; }
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
                //for(const auto &x : v1F) { std::cout << "(" << std::get<0>(x) << ", " << std::get<1>(x) << ", " << std::get<2>(x) << ", " << std::get<3>(x) <<")\n"; }
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
    return;
}

// template<typename Game, typename State, typename Symbol>
// int parityGameSolver<Game, State, Symbol>::val(bool spoiler) {
//     // needed ???
// }

template<typename Game, typename State, typename Symbol>
int parityGameSolver<Game, State, Symbol>::update1(std::tuple<State, State, int, int> v1, Game pg, const uint n1,
                                                   std::map<std::tuple<State, State, Symbol, int, int>, std::set<std::tuple<State, State, int, int>>> &EFA0,
                                                   std::map<std::tuple<State, State, int, int>, std::set<std::tuple<State, State, Symbol, int, int>>> &EFA1) {
    using namespace std;

    if(std::get<2>(v1) == 0) {
        auto successors = pg.succ1(v1, EFA1);
        if(successors.size() == 0) {
            for(auto &itr : EFA0) {
                for(auto &sec : itr.second) {
                    if(std::get<0>(sec) == std::get<0>(v1) &&
                       std::get<1>(sec) == std::get<1>(v1) &&
                       std::get<2>(sec) == std::get<2>(v1)) {
                        EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
                        EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), INF));
                    }
                }
            }

            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            //cout << "(" << get<0>(succ) << ", " << get<1>(succ) << ", " << get<2>(succ) << ", " << get<3>(succ) << ")\n";
            rhos.push_back(get<4>(succ));
        }
        auto maxRho = *max_element(begin(rhos), end(rhos));

        if(maxRho == INF) {
            for(auto &itr : EFA0) {
                for(auto &sec : itr.second) {
                    if(std::get<0>(sec) == std::get<0>(v1) &&
                       std::get<1>(sec) == std::get<1>(v1) &&
                       std::get<2>(sec) == std::get<2>(v1)) {
                        EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
                        EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), INF));
                    }
                }
            }

            return INF;
        }

        if(std::get<3>(v1) != maxRho) {
            for (auto &itr: EFA0) {
                for (auto &sec: itr.second) {
                    if (std::get<0>(sec) == std::get<0>(v1) &&
                        std::get<1>(sec) == std::get<1>(v1) &&
                        std::get<2>(sec) == std::get<2>(v1)) {
                        EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
                        EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), 0));
                    }
                }
            }
        }

        return 0;
    } else if(std::get<2>(v1) == 2) {
        auto successors = pg.succ1(v1, EFA1);
        if(successors.size() == 0) {
//            for(auto &[key, value] : EFA0) {
//                if(1) {
//                    EFA0[key].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
//                    EFA0[key].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), INF));
//                }
//            }

            for(auto &itr : EFA0) {
                for(auto &sec : itr.second) {
                    if(std::get<0>(sec) == std::get<0>(v1) &&
                       std::get<1>(sec) == std::get<1>(v1) &&
                       std::get<2>(sec) == std::get<2>(v1)) {
                        EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
                        EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), INF));
                    }
                }
            }

            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<4>(succ));
        }
        auto maxRho = *min_element(begin(rhos), end(rhos));

        for(auto &itr : EFA0) {
            for(auto &sec : itr.second) {
                if(std::get<0>(sec) == std::get<0>(v1) &&
                   std::get<1>(sec) == std::get<1>(v1) &&
                   std::get<2>(sec) == std::get<2>(v1)) {
                    EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
                    EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), maxRho));
                }
            }
        }

        return maxRho;
    } else { // p(v) is 1
        auto successors = pg.succ1(v1, EFA1);
        if(successors.size() == 0) {
            for(auto &itr : EFA0) {
                for(auto &sec : itr.second) {
                    if(std::get<0>(sec) == std::get<0>(v1) &&
                       std::get<1>(sec) == std::get<1>(v1) &&
                       std::get<2>(sec) == std::get<2>(v1)) {
                        EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
                        EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), INF));
                    }
                }
            }

            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            //cout << "(" << get<0>(succ) << ", " << get<1>(succ) << ", " << get<2>(succ) << ", " << get<3>(succ) << ")\n";
            rhos.push_back(get<4>(succ));
        }
        auto maxRho = *max_element(begin(rhos), end(rhos));

        if(maxRho == INF || maxRho + 1 > n1) {
            for(auto &itr : EFA0) {
                for(auto &sec : itr.second) {
                    if(std::get<0>(sec) == std::get<0>(v1) &&
                       std::get<1>(sec) == std::get<1>(v1) &&
                       std::get<2>(sec) == std::get<2>(v1)) {
                        EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
                        EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), INF));
                    }
                }
            }

            return INF;
        }

        if(std::get<3>(v1) != (maxRho + 1)) {
            for(auto &itr : EFA0) {
                for(auto &sec : itr.second) {
                    if(std::get<0>(sec) == std::get<0>(v1) &&
                       std::get<1>(sec) == std::get<1>(v1) &&
                       std::get<2>(sec) == std::get<2>(v1)) {
                        EFA0[itr.first].erase(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), get<3>(v1)));
                        EFA0[itr.first].insert(make_tuple(get<0>(v1), get<1>(v1), get<2>(v1), (maxRho + 1)));
                    }
                }
            }
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
            for(auto &itr : EFA1) {
                for(auto &sec : itr.second) {
                    if(std::get<0>(sec) == std::get<0>(v0) &&
                       std::get<1>(sec) == std::get<1>(v0) &&
                       std::get<2>(sec) == std::get<2>(v0) &&
                       std::get<3>(sec) == std::get<3>(v0)) {
                        EFA1[itr.first].erase(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<4>(v0)));
                        EFA1[itr.first].insert(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), INF));
                    }
                }
            }

            return INF;
        }
        return 0;
    } else if(std::get<3>(v0) == 2) {
        auto successors = pg.succ0(v0, EFA0);
        if(successors.size() == 0) {
            for(auto &itr : EFA1) {
                for(auto &sec : itr.second) {
                    if(std::get<0>(sec) == std::get<0>(v0) &&
                       std::get<1>(sec) == std::get<1>(v0) &&
                       std::get<2>(sec) == std::get<2>(v0) &&
                       std::get<3>(sec) == std::get<3>(v0)) {
                        EFA1[itr.first].erase(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<4>(v0)));
                        EFA1[itr.first].insert(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), INF));
                    }
                }
            }

            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            rhos.push_back(get<3>(succ));
        }
        auto maxRho = *min_element(begin(rhos), end(rhos));

        for(auto &itr : EFA1) {
            for(auto &sec : itr.second) {
                if(std::get<0>(sec) == std::get<0>(v0) &&
                   std::get<1>(sec) == std::get<1>(v0) &&
                   std::get<2>(sec) == std::get<2>(v0) &&
                   std::get<3>(sec) == std::get<3>(v0)) {
                    EFA1[itr.first].erase(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), get<3>(v0)));
                    EFA1[itr.first].insert(make_tuple(get<0>(v0), get<1>(v0), get<2>(v0), get<3>(v0), maxRho));
                }
            }
        }

        return maxRho;
    } else {
        auto successors = pg.succ0(v0, EFA0);

        // update pred(v) whenever rho(v) would get changed


        if(successors.size() == 0) {
            return INF;
        }

        std::vector<int> rhos;
        for(auto &succ : successors) {
            //cout << "(" << get<0>(succ) << ", " << get<1>(succ) << ", " << get<2>(succ) << ", " << get<3>(succ) << ")\n";
            rhos.push_back(get<3>(succ));
        }
        auto maxRho = *min_element(begin(rhos), end(rhos));

        if(maxRho == INF || maxRho + 1 > n1) return INF;

        return (maxRho + 1);
    }
}

template<typename Game, typename State, typename Symbol>
uint parityGameSolver<Game, State, Symbol>::n_1(Vertex0<State, Symbol> &v0F, Vertex1<State> &v1F) const {
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