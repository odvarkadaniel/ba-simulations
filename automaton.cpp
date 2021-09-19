#include<iostream>
#include<fstream>

#include"automaton.h"
#include"util.h"


std::vector<int> Automaton::getStates() {
    return states;
}

std::vector<int> Automaton::getInitialStates() {
    return initialStates;
}

std::vector<int> Automaton::getAcceptingStates() {
    return acceptingStates;
}

std::vector<std::string> Automaton::getAlphabet() {
    return alphabet;
}

std::multimap<std::pair<int, int>, std::string> Automaton::getTransitions() {
    return transitions;
}

std::multimap<std::pair<int, int>, std::string> Automaton::getReversedTransitions() {
    return reversedTransitions;
}

void Automaton::addState(std::string str, std::vector<int>& stateVector) {
    std::string stateName = "";

    for(int i = 1; str[i] != 93; i++) {
        stateName += str[i];
    }
    stateVector.push_back(std::stoi(stateName));
}

std::string Automaton::getStateForTransition(std::string str) {
    std::string result = "";

    for(int i = 1; str[i] != 93; i++) {
        result += str[i];
    }

    return result;
}

void Automaton::addToAlphabet(std::string str, std::vector<std::string>& alphabetVector) {
    std::string alphabet = "";

    for(int i = 0; i != str.length(); i++) {
        alphabet += str[i];
    }
    alphabetVector.push_back(alphabet);
}

Automaton Automaton::loadAutomaton(std::string filename) {
    using namespace std;

    Automaton omega;
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

            // add char to alphabet vector
            omega.addToAlphabet(v1[0], omega.alphabet);

            // add states to states vector
            omega.addState(v2[0], omega.states);
            omega.addState(v2[1], omega.states);

            // add transitions
            omega.transitions.insert(make_pair(make_pair(stoi(getStateForTransition(v2[0])), stoi(getStateForTransition(v2[1]))), v1[0]));

            // add reversed transitions
            omega.reversedTransitions.insert(make_pair(make_pair(stoi(getStateForTransition(v2[1])), stoi(getStateForTransition(v2[0]))), v1[0]));

        } else {
            error_exit("couldn't parse the given automaton\n");
        }

        line++;
    }

    readFile.close();

    // erase duplicates from states and alphabet
    if(omega.alphabet.empty() || omega.states.empty()) {
        error_exit("alphabet or states vectors are empty.\n");
    }
    eraseDuplicates(omega.alphabet);
    eraseDuplicates(omega.states);


    #ifdef _DEBUG

    cout << "initial states: ";
    for(int i: omega.initialStates) {
        cout << i << ", ";
    }
    
    cout << endl;

    cout << "states: ";
    for(int i: omega.states) {
        cout << i << ", ";
    }

    cout << endl;

    cout << "accepting states: ";
    for(int i: omega.acceptingStates) {
        cout << i << ", ";
    }

    cout << endl;

    cout << "alphabet: ";
    for(string i: omega.alphabet) {
        cout << i << ", ";
    }

    cout << endl;

    cout << "transitions: \n";
    for(const auto& elem : omega.transitions) {
        std::cout << elem.first.first << " " << elem.second << " " << elem.first.second << "\n";
    }

    cout << endl;

    cout << "reversed transitions: \n";
    for(const auto& elem : omega.reversedTransitions) {
        std::cout << elem.first.first << " " << elem.second << " " << elem.first.second << "\n";
    }

    #endif
    
    return omega;
}