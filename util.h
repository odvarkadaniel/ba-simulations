#ifndef UTIL_H
#define UTIL_H

#include<vector>
#include<map>
#include<string>
#include<algorithm>

void error_exit(const char *fmt, ...);

std::vector<std::string> split(std::string original, std::string delimiter);

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

std::vector<int> statesWithoutACC(std::vector<int> &states, std::vector<int> &acceptingStates);

int card(std::multimap<std::pair<int, int>, std::string> transition, int k, std::string alpha);

#endif