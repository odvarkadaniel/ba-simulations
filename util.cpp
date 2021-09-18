#include<stdlib.h>
#include<stdarg.h>
#include<iostream>

#include"util.h"

void error_exit(const char *fmt, ...)
{
	va_list(args);
	fprintf(stderr, "ERROR: ");
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}

std::vector<std::string> split(std::string original, std::string delimiter) {
    using namespace std;
    
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = original.find (delimiter, pos_start)) != string::npos) {
        token = original.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (original.substr (pos_start));
    return res;
}

std::vector<int> statesWithoutACC(std::vector<int> &states, std::vector<int> &acceptingStates) {
    using namespace std;

    vector<int> copy = states;

    for(int i = 0; i < acceptingStates.size(); i++) {
        if(find(copy.begin(), copy.end(), acceptingStates[i]) != copy.end()) {
            copy.erase(remove(copy.begin(), copy.end(), acceptingStates[i]));
        }
    }
    return copy;
}

int card(std::multimap<std::pair<int, int>, std::string> transition, int k, std::string alpha) {
    using namespace std;

    u_int counter = 0;

    for(const auto& tr : transition)  {
        if(tr.first.first == k && tr.second == alpha) {
            counter++;
        }
    }

    return counter;
}
