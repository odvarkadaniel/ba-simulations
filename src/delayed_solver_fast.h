#ifndef DELAYED_SOLVER_FAST_H
#define DELAYED_SOLVER_FAST_H

#include<set>
#include<vector>
#include"delayed_parity_game_solver.h"
#include"delayed_parity_game.h"

#define INF 10

template<class Game, typename State, typename Symbol>
class fastDelayedSolver : public delayedParityGame<State, Symbol> {
public:
    std::set<std::pair<State, State>> fast(Game pg, Automaton<State, Symbol> &omega);

    std::set<std::tuple<bool, State, State, Symbol, int, int>> pred1(std::tuple<bool, State, State, int, int> v1, Game pg);
    int incr1(std::tuple<bool, State, State, int, int> v1, int B);
    int val1(Game pg, std::tuple<bool, State, State, int, int> v1);
    int cnt1(Game pg, std::tuple<bool, State, State, int, int> v1);

    std::set<std::tuple<bool, State, State, int, int>> pred0(std::tuple<bool, State, State, Symbol, int, int> v0, Game pg);
    int incr0(std::tuple<bool, State, State, Symbol, int, int> v0, int B);
    int val0(Game pg, std::tuple<bool, State, State, Symbol, int, int> v0);
    int cnt0(Game pg, std::tuple<bool, State, State, Symbol, int, int> v0);

    uint n1(std::set<std::tuple<bool, State, State, Symbol, int, int>> &v0D, std::set<std::tuple<bool, State, State, int, int>> &v1D);
    uint n1Count;
private:
    std::map<std::tuple<bool, State, State, Symbol, int, int>, std::vector<int>> dataZero;
    std::map<std::tuple<bool, State, State, int, int>, std::vector<int>> dataOne;
    std::set<std::tuple<bool, State, State, int, int>> L1;
    std::set<std::tuple<bool, State, State, Symbol, int, int>> L0;
};

template<class Game, typename State, typename Symbol>
std::set<std::pair<State, State>> fastDelayedSolver<Game, State, Symbol>::fast(Game pg, Automaton<State, Symbol> &omega) {
    auto v0D = pg.getv0D();
    auto v1D = pg.getv1D();
    auto EDA0 = pg.getEDA0();
    auto EDA1 = pg.getEDA1();

    n1Count = n1(v0D, v1D);

    for(const auto &v0 : v0D) {
        auto succs = EDA0.find(v0);
        int number = succs->second.size();
        std::vector<int> insert;
        if(succs == EDA0.end()) {
            insert = {0, 0, INF}; // B(v), C(v), rho(v)
        } else {
            insert = {0, number, 0}; // B(v), C(v), rho(v)
        }
        dataZero[v0] = insert;
        if(std::get<4>(v0) == 1) L0.insert(v0);
    }

    for(const auto &v1 : v1D) {
        auto succs = EDA1.find(v1);
        int number = succs->second.size();

        for(const auto &s : succs->second) {
            auto it = EDA0.find(s);
            if(it == EDA0.end()) {
                std::vector<int> insert = {0, number, INF};
                dataOne[v1] = insert;
                break;
            } else {
                std::vector<int> insert = {0, number, 0};
                dataOne[v1] = insert;
            }
        }
        if(std::get<3>(v1) == 1) L1.insert(v1);
    }
    bool changed = true;
    while(changed) {
        changed = false;
        for(const auto &v0 : dataZero) {
            auto succ = pg.succ0(v0.first, EDA0);
            if(succ.size() == 1) {
                for(const auto &s : succ) {
                    auto it = dataOne.find(s);
                    if(it->second[2] == INF && v0.second[2] != INF) {
                        dataZero[v0.first][2] = INF;
                        changed = true;
                    }
                }
            }
        }
        for(const auto &v1 : dataOne) {
            auto succ = pg.succ1(v1.first, EDA1);
            for(const auto &s : succ) {
                auto it = dataZero.find(s);
                if(it->second[2] == INF && v1.second[2] != INF) {
                    dataOne[v1.first][2] = INF;
                    changed = true;
                }
            }
        }
    }

    std::vector<std::tuple<bool, State, State, Symbol, int, int>> temp_v0;
    std::vector<std::tuple<bool, State, State, int, int>> temp_v1;
    std::vector<std::tuple<bool, State, State, Symbol, int, int>> temp_pred1;
    std::vector<std::tuple<bool, State, State, int, int>> temp_pred0;
    while(!L0.empty() || !L1.empty()) {
        for(auto &v1 : L1) {
            auto it = dataOne.find(v1);
            int t = it->second[2];
            it->second[0] = val1(pg, v1); // B
            it->second[1] = cnt1(pg, v1); // C
            it->second[2] = incr1(v1, it->second[0]);
            auto pred = pred1(v1, pg);

            for(const auto &w : pred) {
                auto wInfo = dataZero.find(w);
                if(std::get<4>(wInfo->first) != 0 && it->second[2] > wInfo->second[0]) {
                    if ((std::get<4>(wInfo->first) != 0 && t == wInfo->second[0]) ||
                        (std::get<4>(wInfo->first) == 0 && it->second[0] == 0)) {
                        if (wInfo->second[1] > 1) {
                            wInfo->second[1] -= 1;
                        } else if (wInfo->second[1] == 1) {
                            if (wInfo->second[2] != INF)
                                L0.insert({wInfo->first});
                        }
                    }
                }
            }
            temp_v1.push_back(v1);
        }

        for(const auto &insert : temp_v1) {
            L1.erase(insert);
        }
        temp_v1.clear();

        for(const auto &v0 : L0) {
            auto it = dataZero.find(v0);
            int t = it->second[2];
            it->second[0] = val0(pg, v0); // B
            it->second[1] = cnt0(pg, v0); // C
            it->second[2] = incr0(v0, it->second[0]);
            auto pred = pred0(v0, pg);

            for(const auto &w : pred) {
                auto wInfo = dataOne.find(w);
                if(std::get<3>(wInfo->first) != 0 && it->second[2] > wInfo->second[0]) {
                    if(wInfo->second[2] != INF)
                        L1.insert({wInfo->first});
                }
            }
            temp_v0.push_back(v0);
        }

        for(const auto &insert : temp_v0) {
            L0.erase(insert);
        }
        temp_v0.clear();
    }

    std::set<std::pair<State, State>> result;

    for(const auto &[key, value] : dataOne) {
        using namespace std;
        if(value[2] < INF) {
            if (get<0>(key) == 1 && omega.isAcceptingState(get<1>(key)) && !omega.isAcceptingState(get<2>(key))) {
                result.insert(std::make_pair(std::get<1>(key), std::get<2>(key)));
            } else {
                if(get<0>(key) == 0)
                    result.insert(std::make_pair(std::get<1>(key), std::get<2>(key)));
            }
            if(get<1>(key) == get<2>(key)) {
                result.insert(std::make_pair(std::get<1>(key), std::get<2>(key)));
            }
        }
    }

    return result;
}

template<class Game, typename State, typename Symbol>
std::set<std::tuple<bool, State, State, Symbol, int, int>>
fastDelayedSolver<Game, State, Symbol>::pred1(std::tuple<bool, State, State, int, int> v1,
                                           Game pg) {
    std::set<std::tuple<bool, State, State, Symbol, int, int>> predSet;
    auto EDA0 = pg.getEDA0();
    for(const auto &[key, value] : EDA0) {
        for(const auto &v : value) {
            if(v == v1) predSet.insert(key);
        }
    }

    return predSet;
}

template<class Game, typename State, typename Symbol>
int fastDelayedSolver<Game, State, Symbol>::incr1(std::tuple<bool, State, State, int, int> v1, int B) {
    if(std::get<3>(v1) == 1) {
        if((B + 1) > n1Count) {
            return INF;
        }
        return (B + 1);
    } else {
        return B;
    }
}

template<class Game, typename State, typename Symbol>
int fastDelayedSolver<Game, State, Symbol>::val1(Game pg, std::tuple<bool, State, State, int, int> v1) {
    auto EDA1 = pg.getEDA1();
    auto succs = pg.succ1(v1, EDA1);

    std::vector<int> rhos;
    for(const auto &s : succs) {
        auto it = dataZero.find(s);
        rhos.push_back(it->second[2]);
    }

    auto maxRho = *std::max_element(rhos.begin(), rhos.end());

    int prio = std::get<3>(v1);

    if(prio == 0) {
        if(maxRho == INF) return INF;
        return 0;
    } else {
        return maxRho;
    }
}

template<class Game, typename State, typename Symbol>
int fastDelayedSolver<Game, State, Symbol>::cnt1(Game pg, std::tuple<bool, State, State, int, int> v1) {
    auto EDA1 = pg.getEDA1();
    auto succs = pg.succ1(v1, EDA1);
    int counter = 0;

    auto valNumber = val1(pg, v1);

    for(const auto &s : succs) {
        auto it = dataZero.find(s);
        int prio = std::get<3>(v1);
        int rho = it->second[2];
        if(prio == 1) {
            if(rho == valNumber) counter++;
        } else if(prio == 2) {
            if(rho == valNumber) counter++;
        } else { // p(v) = 0
            if(rho == INF) {
                if(rho == valNumber) {
                    counter++;
                }
            } else {
                if(0 == valNumber) counter++;
            }
        }
    }
    return counter;
}

template<typename Game, typename State, typename Symbol>
uint fastDelayedSolver<Game, State, Symbol>::n1(std::set<std::tuple<bool, State, State, Symbol, int, int>> &v0D, std::set<std::tuple<bool, State, State, int, int>> &v1D) {
    uint counter = 0;

    for(const auto &x : v0D) {
        if(std::get<4>(x) == 1) counter++;
    }
    for(const auto &x : v1D) {
        if(std::get<3>(x) == 1) counter++;
    }

    return counter;
}

template<class Game, typename State, typename Symbol>
int fastDelayedSolver<Game, State, Symbol>::incr0(std::tuple<bool, State, State, Symbol, int, int> v0, int B) {
    if(std::get<4>(v0) == 1) {
        if((B + 1) > n1Count) {
            return INF;
        }
        return (B + 1);
    } else {
        return B;
    }
}

template<class Game, typename State, typename Symbol>
std::set<std::tuple<bool, State, State, int, int>>
fastDelayedSolver<Game, State, Symbol>::pred0(std::tuple<bool, State, State, Symbol, int, int> v0, Game pg) {
    std::set<std::tuple<bool, State, State, int, int>> predSet;
    auto EDA1 = pg.getEDA1();
    for(const auto &[key, value] : EDA1) {
        for(const auto &v : value) {
            if(v == v0) predSet.insert(key);
        }
    }

    return predSet;
}

template<class Game, typename State, typename Symbol>
int fastDelayedSolver<Game, State, Symbol>::val0(Game pg, std::tuple<bool, State, State, Symbol, int, int> v0) {
    auto EDA0 = pg.getEDA0();
    auto succs = pg.succ0(v0, EDA0);

    std::vector<int> rhos;
    for(const auto &s : succs) {
        auto it = dataOne.find(s);
        rhos.push_back(it->second[2]);
    }

    auto minRho = *std::min_element(rhos.begin(), rhos.end());

    int prio = std::get<4>(v0);
    if(prio == 0) {
        if(minRho == INF) return INF;
        return 0;
    } else {
        return minRho;
    }
}

template<class Game, typename State, typename Symbol>
int fastDelayedSolver<Game, State, Symbol>::cnt0(Game pg, std::tuple<bool, State, State, Symbol, int, int> v0) {
    auto EDA0 = pg.getEDA0();
    auto succs = pg.succ0(v0, EDA0);
    int counter = 0;

    for(const auto &s : succs) {
        auto it = dataOne.find(s);
        auto valNumber = val0(pg, v0);
        int prio = std::get<4>(v0);
        int rho = it->second[2];
        if(prio == 1) {
            if(rho == valNumber) counter++;
        } else if(prio == 2) {
            if(rho == valNumber) counter++;
        } else { // p(v) = 0
            if(rho == INF) {
                if(rho == valNumber) {
                    counter++;
                }
            } else {
                if(0 == valNumber) counter++;
            }
        }
    }
    return counter;
}

#endif