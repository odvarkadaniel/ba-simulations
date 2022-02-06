#include<iostream>
#include<fstream>

#include"automaton.h"
#include"util.h"

template<>
bool Automaton<std::string, std::string>::isInTransition(std::map<std::pair<std::string, std::string>, std::set<std::string>>& transition, std::string t1, std::string alpha) {
    for(const auto& [p, val] : transition) {
        if (t1 == p.first && p.first == alpha) {
            return true;
        }
    }

    return false;
}

template<>
void Automaton<std::string, std::string>::addState(std::string str, std::set<std::string>& stateVector) {
    std::string stateName = "";

    for(int i = 1; str[i] != 93; i++) {
        stateName += str[i];
    }
    stateVector.insert({stateName});
}

template<>
std::string Automaton<std::string, std::string>::getStateForTransition(std::string str) {
    std::string result = "";

    for(int i = 1; str[i] != 93; i++) {
        result += str[i];
    }

    return result;
}

template<>
void Automaton<std::string, std::string>::addToAlphabet(std::string str, std::set<std::string>& alphabetVector) {
    std::string alphabet = "";

    for(int i = 0; i != str.length(); i++) {
        alphabet += str[i];
    }
    alphabetVector.insert({alphabet});
}

template<>
Automaton<std::string, std::string> Automaton<std::string, std::string>::loadAutomaton(std::string filename) {
    using namespace std;

    Automaton<string, string> omega;
    fstream readFile(filename);
    string temp, s, s1, s2;
    int line = 1;
    string delimiterTransition = "->";
    string delimeterComma = ",";

    while(getline(readFile, temp)) {

        const char *tempc = temp.c_str();

        if(line == 1 && tempc[0] == 91) { // initial states
            omega.addState(temp, omega.initialStates);
        } else if(line != 1 && tempc[0] == 91) { // accepting states
            omega.addState(temp, omega.acceptingStates);
        } else if(line != 1) {
            vector<string> v1 = split(temp, delimeterComma); // v1[0] is a char from alphabet
            vector<string> v2 = split(v1[1], delimiterTransition);

            // add char to alphabet set
            omega.addToAlphabet(v1[0], omega.alphabet);

            // add states to states set
            omega.addState(v2[0], omega.states);
            omega.addState(v2[1], omega.states);

            // add transitions
            omega.transitions[make_pair(getStateForTransition(v2[0]), v1[0])].insert(getStateForTransition(v2[1]));
            //omega.addNewTransition(make_pair(getStateForTransition(v2[0]), v1[0]), set<string>({getStateForTransition(v2[1])}));
            //omega.transitions.insert({make_pair(getStateForTransition(v2[0]), v1[0]), set<string>({getStateForTransition(v2[1])})});
            //omega.transitions[make_pair(getStateForTransition(v2[0]), v1[0])] = set<string>({getStateForTransition(v2[1])});
            
            // add reversed transitions
            omega.reversedTransitions[make_pair(getStateForTransition(v2[1]), v1[0])].insert(getStateForTransition(v2[0]));
            //omega.addNewRevTransition(make_pair(getStateForTransition(v2[1]), v1[0]), set<string>({getStateForTransition(v2[0])}));
            //omega.reversedTransitions.insert({make_pair(getStateForTransition(v2[1]), v1[0]), set<string>({getStateForTransition(v2[0])})});

        } else {
            error_exit("couldn't parse the given automaton\n");
        }

        line++;
    }

    readFile.close();

    // erase duplicates from states and alphabet
    // eraseDuplicates(omega.alphabet);
    // eraseDuplicates(omega.states);


    #ifdef _DEBUG

    cout << "initial states: ";
    for(const auto &i: omega.initialStates) {
        cout << i << ", ";
    }
    
    cout << endl;

    cout << "states: ";
    for(const auto &i: omega.states) {
        cout << i << ", ";
    }

    cout << endl;

    cout << "accepting states: ";
    for(const auto &i: omega.acceptingStates) {
        cout << i << ", ";
    }

    cout << endl;

    cout << "alphabet: ";
    for(const auto &i: omega.alphabet) {
        cout << i << ", ";
    }

    cout << endl;

    cout << "transitions: \n";
    for(const auto& elem : omega.transitions) {
        for(const auto& e : elem.second) {
            std::cout << elem.first.first << " " << elem.first.second << " " << e << endl;
        }
    }

    cout << endl;

    cout << "reversed transitions: \n";
    for(const auto& elem : omega.reversedTransitions) {
        for(const auto& e : elem.second)
            std::cout << elem.first.first << " " << elem.first.second << " " << e << endl;
    }

    cout << endl;

    #endif
    
    return omega;
}