#ifndef UTIL_H
#define UTIL_H

#include<vector>
#include<map>
#include<string>
#include<algorithm>
#include<set>

// functions
void error_exit(const char *fmt, ...);
std::vector<std::string> split(std::string original, std::string delimiter);

/**
 * Erases all duplicates from a vector
 * @tparam S
 * @tparam I
 * @param vec
 */
template<typename S, typename I>
void eraseDuplicates(std::vector<S, I> &vec) { 
    using namespace std;

    if(vec.empty()) {
        error_exit("vector of alphabet/states is empty\n");
        return;
    }
    
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());

    return;
}

/**
 * Removes accepting states from Q
 * @tparam State
 * @param states
 * @param acceptingStates
 * @return
 */
template<typename State>
std::set<State> statesWithoutACC(std::set<State> &states, std::set<State> &acceptingStates) {
    using namespace std;

    set<State> copy = states;

    for(const auto& accSt : acceptingStates) {
        copy.erase(accSt);
    }
    return copy;
}

/**
 * Computes a card number needed for direct simulation
 * Number of transitions from a state S with a symbol A
 * @tparam State
 * @tparam Symbol
 * @param transition
 * @param k
 * @param alpha
 * @return
 */
template<typename State, typename Symbol>
int card(std::map<std::pair<State, Symbol>, std::set<State>>& transition, State k, Symbol alpha) {
    using namespace std;

    u_int counter = 0;

    for(const auto& [p, val] : transition)  {
        if(p.first == k && p.second == alpha) {
            for(const auto& setval : val)
                counter++;
        }
    }

    return counter;
}


#endif