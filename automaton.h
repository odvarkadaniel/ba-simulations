#ifndef AUTOMATON_H
#define AUTOMATON_H

#include<vector>
#include<string>
#include<map>


class Automaton {
    public:
        Automaton loadAutomaton(std::string filename);

        std::vector<int> getStates() { return states; }
        std::vector<int> getInitialStates() { return initialStates; }
        std::vector<int> getAcceptingStates() { return acceptingStates; }
        std::vector<std::string> getAlphabet() { return alphabet; }
        std::multimap<std::pair<int, int>, std::string> getTransitions() { return transitions; }
        std::multimap<std::pair<int, int>, std::string> getReversedTransitions() { return reversedTransitions; }

        void addToAlphabet(std::string str, std::vector<std::string>& alphabetVector);
        void addState(std::string str, std::vector<int>& stateVector);
        std::string getStateForTransition(std::string str);
        bool isInTransition(std::multimap<std::pair<int, int>, std::string>& transition, int t1, std::string alpha);
    protected:
        std::vector<std::string> alphabet;
        std::vector<int> states;
        std::vector<int> initialStates;
        std::vector<int> acceptingStates;
        std::multimap<std::pair<int, int>, std::string> transitions;
        std::multimap<std::pair<int, int>, std::string> reversedTransitions;
};

#endif