#ifndef AUTOMATON_H
#define AUTOMATON_H

#include<vector>
#include<string>
#include<map>
#include<set>

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

//transition functions
template<typename State, typename Symbol> using Delta = std::map<std::pair<State, Symbol>, std::set<State>>;
template<typename State, typename Symbol> using VecTrans = std::vector<Transition<State, Symbol>>;

template<typename State, typename Symbol>
class Automaton {
    public:
        typedef std::set<State> SetStates;
        typedef std::set<Symbol> SetSymbols;
        // StateRelation useful for direct simulation
        typedef std::set<std::pair<State, State>> StateRelation;

        Automaton loadAutomaton(std::string filename);

        SetStates getStates() { return this->states; }
        SetStates getInitialStates() { return this->initialStates; }
        SetStates getAcceptingStates() { return this->acceptingStates; }
        SetSymbols getAlphabet() { return this->alphabet; }
        Delta<State, Symbol> getTransitions() { return this->transitions; }
        Delta<State, Symbol> getReversedTransitions() { return this->reversedTransitions; }

        void addToAlphabet(std::string str, std::set<std::string>& alphabetVector);
        void addState(std::string str, std::set<std::string>& stateVector);
        std::string getStateForTransition(std::string str);
        bool isInTransition(std::map<std::pair<std::string, std::string>, std::set<std::string>>& transition, std::string t1, std::string alpha);
        void addNewTransition(std::pair<State, Symbol> src, std::set<State> dst);
        void addNewRevTransition(std::pair<State, Symbol> src, std::set<State> dst);
    protected:
        SetStates states;
        SetStates initialStates;
        SetStates acceptingStates;
        SetSymbols alphabet;
        Delta<State, Symbol> transitions;
        Delta<State, Symbol> reversedTransitions;
};


template<typename State, typename Symbol>
void Automaton<State, Symbol>::addNewTransition(std::pair<State, Symbol> src, std::set<State> dst){
    this->transitions.insert({src, dst});
}

template<typename State, typename Symbol>
void Automaton<State, Symbol>::addNewRevTransition(std::pair<State, Symbol> src, std::set<State> dst){
    this->reversedTransitions.insert({src, dst});
}

#endif