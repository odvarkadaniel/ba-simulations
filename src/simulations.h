#ifndef SIMULATIONS_H
#define SIMULATIONS_H

#include<queue>

#include"automaton.h"
#include"util.h"


/**
 * Class that holds stuff for simulations
 * @tparam State
 * @tparam Symbol
 */
template<typename State, typename Symbol>
class Simulation : public Automaton<State, Symbol> {
    public:
        // StateRelation useful for direct simulation
        typedef std::set<std::pair<State, State>> StateRelation;

        // flag for dotfile output of the simulation
        bool dotfile = false;

        StateRelation directSimulationRelation(Automaton<State, Symbol> &a);

};

/**
 * Computes a COMPLEMENT to a preorder
 * This complement is returned as a set of pairs
 * @tparam State
 * @tparam Symbol
 * @param a
 * @return
 */
template<typename State, typename Symbol>
std::set<std::pair<State, State>> Simulation<State, Symbol>::directSimulationRelation(Automaton<State, Symbol> &a) {
    using namespace std;

    set<string> states = a.getStates();
    set<string> acceptingStates = a.getAcceptingStates();
    set<string> alphabet = a.getAlphabet();
    Delta<string, string> transitions = a.getTransitions();
    Delta<string, string> reversedTransitions = a.getReversedTransitions();

    set<pair<State, State>> omega; // complement to the preorder
    queue<pair<State, State>> q; // queue with the states

    set<State> k, j;

    set<State> statesWoACC = statesWithoutACC(states, acceptingStates);

    // for(const auto& i : acceptingStates) {
    //     for(const auto& j : statesWoACC) {
    //         q.push(make_pair(i, j));
    //         omega.insert({make_pair(i, j)});
    //         cout << "(" << i << ", " << j << ")" << " ";
    //     }
    // }

    for(const auto & i : states) {
        for(const auto& j : states) {
            for(const auto& alpha : alphabet) {
                if(((find(acceptingStates.begin(), acceptingStates.end(), i) != acceptingStates.end()) && (find(acceptingStates.begin(), acceptingStates.end(), j) == acceptingStates.end()))
                   || (a.isInTransition(i, alpha) && (!a.isInTransition(j, alpha)))) {
                    q.push(make_pair(i, j));
                    omega.insert({make_pair(i, j)});
                    break;
                }
            }
        }
    }

    while(!q.empty()) {
        auto m = q.front().first;
        auto n = q.front().second;

        q.pop();

        for(const auto& alpha : alphabet) {
            k.clear();
            j.clear();
            for(const auto& [pair, value] : reversedTransitions) {
                if (n == pair.first && pair.second == alpha) {
                    for(const auto& t : value)
                        k.insert({t});
                }
            }
            for(const auto& memberK : k) {
                if(!k.empty() && card(transitions, memberK, alpha) == 1) {
                    for(const auto& [p, val] : reversedTransitions) {
                        if (m == p.first && p.second == alpha) {
                            for(const auto& t : val)
                                j.insert({t});
                        }
                    }

                    for(const auto& one : j) {
                        if(find(omega.begin(), omega.end(), make_pair(one, memberK)) == omega.end()) {
                            omega.insert({make_pair(one, memberK)});
                            q.push(make_pair(one, memberK));
                        }
                    }
                }
            }
        }
    }

    std::set<std::pair<State, State>> result;
    for(const auto &s1 : states) {
        for(const auto &s2 : states) {
                result.insert(std::make_pair(s1, s2));
        }
    }

    // for(const auto &complement : omega) {
    //     for(const auto &res : result) {
    //         if(res.first == complement.first && res.second == complement.second) {
    //             result.erase(std::make_pair(res.first, res.second));
    //         }
    //     }
    // }

//    for(const auto &r : result) {
//        std::cout << r.first << r.second << "\n";
//    }

    return omega;
}

#endif