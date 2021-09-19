#ifndef AUTOMATON_H
#define AUTOMATON_H

#include<vector>
#include<string>
#include<map>


class Automaton {
    public:
        Automaton loadAutomaton(std::string filename);
        std::vector<int> getStates();
        std::vector<int> getInitialStates();
        std::vector<int> getAcceptingStates();
        std::vector<std::string> getAlphabet();
        std::multimap<std::pair<int, int>, std::string> getTransitions();
        std::multimap<std::pair<int, int>, std::string> getReversedTransitions();

        void addToAlphabet(std::string str, std::vector<std::string>& alphabetVector);
        void addState(std::string str, std::vector<int>& stateVector);
        std::string getStateForTransition(std::string str);
    protected:
        std::vector<std::string> alphabet;
        std::vector<int> states;
        std::vector<int> initialStates;
        std::vector<int> acceptingStates;
        std::multimap<std::pair<int, int>, std::string> transitions;
        std::multimap<std::pair<int, int>, std::string> reversedTransitions;
};

#endif