#include<iostream>
#include<fstream>

#include"automaton.h"
#include"util.h"


bool Automaton::isInTransition(std::multimap<std::pair<int, int>, std::string>& transition, int t1, std::string alpha) {
    for(const auto& [p, val] : transition) {
        if(t1 == p.first && val == alpha) {
            return true;
        }
    }

    return false;
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

    cout << endl;

    #endif
    
    return omega;
}