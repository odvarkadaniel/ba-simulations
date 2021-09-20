#include<iostream>
#include<queue>

#include"simulations.h"
#include"util.h"

void uniqueQueue(std::queue<std::pair<int, int>> &q) {
    using namespace std;

    vector<pair<int, int>> temp;
    while(!q.empty()) {
        temp.push_back(q.front());
        q.pop();
    }

    eraseDuplicates(temp);

    #ifdef _DEBUG

    cout << "\nInitialized queue: ";
    for(const auto& x : temp) {
        cout << "(" << x.first << ", " << x.second << ")" << " ";
    }

    #endif

    for(const auto& x : temp) {
        q.push(x);
    }
}

std::vector<std::pair<int, int>> Simulation::directSimulationRelation(Automaton &a) {
    using namespace std;

    vector<int> states = a.getStates();
    vector<int> acceptingStates = a.getAcceptingStates();
    vector<string> alphabet = a.getAlphabet();
    multimap<pair<int, int>, string> transitions = a.getTransitions();
    multimap<pair<int, int>, string> reversedTransitions = a.getReversedTransitions();
    
    vector<pair<int, int>> omega; // complement to the preorder
    queue<pair<int, int>> q; // queue with the states

    vector<int> k, j;

    vector<int> statesWoACC = statesWithoutACC(states, acceptingStates);

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
                    omega.push_back(make_pair(i, j));
                }
            }
        }
    }

    uniqueQueue(q);
    eraseDuplicates(omega);

    while(!q.empty()) {
        auto m = q.front().first;
        auto n = q.front().second;

        q.pop();

        for(const auto& alpha : alphabet) {
            k.clear();
            j.clear();
            for(const auto& [pair, value] : reversedTransitions) {
                if(n == pair.first && value == alpha) {
                    k.push_back(pair.second);
                }
            }
            for(const auto& memberK : k) {
                if(!k.empty() && card(transitions, memberK, alpha) == 1) {
                    for(const auto& [p, val] : reversedTransitions) {
                        if(m == p.first && val == alpha) {
                            j.push_back(p.second);
                        }
                    }
                    if(!j.empty())
                        eraseDuplicates(j);

                    for(const auto& one : j) {
                        if(find(omega.begin(), omega.end(), make_pair(one, memberK)) == omega.end()) {
                            omega.push_back(make_pair(one, memberK));
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
