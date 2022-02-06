#ifndef SIMULATIONS_H
#define SIMULATIONS_H

#include<queue>
#include"automaton.h"
#include"util.h"

template<typename State, typename Symbol>
class Simulation : public Automaton<State, Symbol> {
    public:
        // StateRelation useful for direct simulation
        typedef std::set<std::pair<State, State>> StateRelation;

         StateRelation directSimulationRelation(Automaton<State, Symbol> &a);

};


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

    vector<State> k, j;

    set<State> statesWoACC = statesWithoutACC(states, acceptingStates);

    // for(const auto& i : acceptingStates) {
    //     for(const auto& j : statesWoACC) {
    //         q.push(make_pair(i, j));
    //         omega.push_back(make_pair(i, j));
    //         cout << "(" << i << ", " << j << ")" << " ";
    //     }
    // }

    for(const auto & i : states) {
        for(const auto& j : states) {
            for(const auto& alpha : alphabet) {
                if(((find(acceptingStates.begin(), acceptingStates.end(), i) != acceptingStates.end()) && (find(acceptingStates.begin(), acceptingStates.end(), j) == acceptingStates.end()))
                   || (a.isInTransition(transitions, i, alpha) && !a.isInTransition(transitions, j, alpha))) {
                    q.push(make_pair(i, j));
                    omega.insert({make_pair(i, j)});
                    break;
                }
            }
        }
    }

    //eraseDuplicates(omega); asi nepotrebuju no more

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
                        k.push_back(t);
                }
            }
            for(const auto& memberK : k) {
                if(!k.empty() && card(transitions, memberK, alpha) == 1) {
                    for(const auto& [p, val] : reversedTransitions) {
                        if (m == p.first && p.second == alpha) {
                            for(const auto& t : val)
                                j.push_back(t);
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

    cout << "\nDirect simulation relation: ";
    for ( const auto& token : omega )
        cout << "(" << token.first << ", " << token.second << ")" << " ";
    cout << endl;

    return omega;
}

#endif