#include<iostream>
#include<fstream>
#include<algorithm>

#include"automaton.h"
#include"util.h"

template<>
bool Automaton<std::string, std::string>::isAcceptingState(std::string s1) {
    auto accSt = this->getAcceptingStates();
    for(const auto &acc : accSt) {
        if(s1 == acc) return true;
    }
    return false;
}

/**
 * Checks whether such transition exists - used for direct simulation calculation
 * @param transition
 * @param t1
 * @param alpha
 * @return
 */
template<>
bool Automaton<std::string, std::string>::isInTransition(std::string t1, std::string alpha) {
    for(const auto& [p, val] : this->transitions) {
        if (t1 == p.first && p.second == alpha) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Adds state to a state set
 * 
 * @param str
 * @param stateVector
 */
template<>
void Automaton<std::string, std::string>::addState(std::string str) {
    std::string stateName = "";

    for(int i = 1; str[i] != 93; i++) {
        if(isalpha(str[i]) || isdigit(str[i]))
            stateName += str[i];
    }
    this->states.insert({stateName});
}

/**
 * @brief Add state to an accepting states set
 * 
 * @tparam  
 * @param str 
 */
template<>
void Automaton<std::string, std::string>::addAcceptingState(std::string str) {
    std::string stateName = "";

    for(int i = 1; str[i] != 93; i++) {
        if(isalpha(str[i]) || isdigit(str[i]))
            stateName += str[i];
    }
    this->acceptingStates.insert({stateName});
}

/**
 * @brief Adds state to an initial states set
 * 
 * @tparam  
 * @param str 
 */
template<>
void Automaton<std::string, std::string>::addInitialState(std::string str) {
    std::string stateName = "";

    for(int i = 1; str[i] != 93; i++) {
        if(isalpha(str[i]) || isdigit(str[i]))
            stateName += str[i];
    }
    this->initialStates.insert({stateName});
}

/**
 * Parses the input as an transition
 * Gets rid of [], e.g. [3] is transformed into 3
 * @param str
 * @return
 */
template<>
std::string Automaton<std::string, std::string>::getStateForTransition(std::string str) {
    std::string result = "";

    for(int i = 1; str[i] != 93; i++) {
        result += str[i];
    }

    return result;
}

/**
 * Adds an element to an alphabet
 * @param str
 * @param alphabetVector
 */
template<>
void Automaton<std::string, std::string>::addToAlphabet(std::string str) {
    std::string alphabet = "";

    for(int i = 0; i != str.length(); i++) {
        alphabet += str[i];
    }
    this->alphabet.insert({alphabet});
}

/**
 * Loads an automaton from .ba formatted file into Automaton class
 * @param filename
 * @return
 */
template<>
Automaton<std::string, std::string> Automaton<std::string, std::string>::loadAutomaton(std::string filename) {
    using namespace std;

    Automaton<string, string> omega;
    string temp, s, s1, s2;
    int line = 1;
    string delimiterTransition = "->";
    string delimeterComma = ",";
    
    fstream readFile(filename);
    if(readFile.fail()) {
        cerr << "The file <" << filename << "> doesn't exist!\n";
        exit(1);
    }

    while(getline(readFile, temp)) {

        const char *tempc = temp.c_str();

        if(line == 1 && tempc[0] == 91) { // initial states
            omega.addInitialState(temp);
        } else if(line != 1 && tempc[0] == 91) { // accepting states
            omega.addAcceptingState(temp);
        } else if(line != 1) {
            vector<string> v1 = split(temp, delimeterComma); // v1[0] is a char from alphabet
            vector<string> v2 = split(v1[1], delimiterTransition);

            // add char to alphabet set
            omega.addToAlphabet(v1[0]);
            
            // remove whitespaces from states
            for(auto& str : v2) 
                std::remove_if(str.begin(), str.end(), ::isspace);

            // add states to states set
            omega.addState(v2[0]);
            omega.addState(v2[1]);

            // add transitions
            omega.transitions[make_pair(getStateForTransition(v2[0]), v1[0])].insert(getStateForTransition(v2[1]));
            
            // add reversed transitions
            omega.reversedTransitions[make_pair(getStateForTransition(v2[1]), v1[0])].insert(getStateForTransition(v2[0]));

        } else {
            error_exit("couldn't parse the given automaton\n");
        }

        line++;
    }

    readFile.close();

    #ifdef _DEBUG

//    cout << "initial states: ";
//    for(const auto &i: omega.initialStates) {
//        cout << i << ", ";
//    }
//
//    cout << endl;
//
//    cout << "states: ";
//    for(const auto &i: omega.states) {
//        cout << i << ", ";
//    }
//
//    cout << endl;
//
//    cout << "accepting states: ";
//    for(const auto &i: omega.acceptingStates) {
//        cout << i << ", ";
//    }
//
//    cout << endl;
//
//    cout << "alphabet: ";
//    for(const auto &i: omega.alphabet) {
//        cout << i << ", ";
//    }
//
//    cout << endl;
//
//    cout << "transitions: \n";
//    for(const auto& elem : omega.transitions) {
//        for(const auto& e : elem.second) {
//            std::cout << elem.first.first << " " << elem.first.second << " " << e << endl;
//        }
//    }
//
//    cout << endl;
//
//    cout << "reversed transitions: \n";
//    for(const auto& elem : omega.reversedTransitions) {
//        for(const auto& e : elem.second)
//            std::cout << elem.first.first << " " << elem.first.second << " " << e << endl;
//    }
//
//    cout << endl;

    #endif
    
    return omega;
}