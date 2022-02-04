#ifndef AUTOMATON_H
#define AUTOMATON_H

#include<vector>
#include<string>
#include<map>

template <typename State, typename Symbol>
struct Transition {
  State from;
  State to;
  Symbol symbol;

  bool operator==(const Transition& other) const
  {
    return other.from == from && other.to == to && other.symbol == symbol;
  }
};

class Automaton {
    public:
        Automaton loadAutomaton(std::string filename);

        std::vector<int> getStates() { return this->states; }
        std::vector<int> getInitialStates() { return this->initialStates; }
        std::vector<int> getAcceptingStates() { return this->acceptingStates; }
        std::vector<std::string> getAlphabet() { return this->alphabet; }
        std::multimap<std::pair<int, int>, std::string> getTransitions() { return this->transitions; }
        std::multimap<std::pair<int, int>, std::string> getReversedTransitions() { return this->reversedTransitions; }

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