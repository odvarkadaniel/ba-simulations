#ifndef AUTOMATON_H
#define AUTOMATON_H

#include<vector>
#include<string>
#include<map>
#include<set>

//transition function
template<typename State, typename Symbol> using Delta = std::map<std::pair<State, Symbol>, std::set<State>>;

/**
 * Class that holds the whole Buchi automaton
 * @tparam State
 * @tparam Symbol
 */
template<typename State, typename Symbol>
class Automaton {
    public:
        typedef std::set<State> SetStates;
        typedef std::set<Symbol> SetSymbols;

        Automaton loadAutomaton(std::string filename);

        //getters
        SetStates getStates() { return this->states; }
        SetStates getInitialStates() { return this->initialStates; }
        SetStates getAcceptingStates() { return this->acceptingStates; }
        SetSymbols getAlphabet() { return this->alphabet; }
        Delta<State, Symbol> getTransitions() { return this->transitions; }
        Delta<State, Symbol> getReversedTransitions() { return this->reversedTransitions; }

        void addToAlphabet(Symbol str);
        void addState(State str);
        void addInitialState(State str);
        void addAcceptingState(State str);
        std::string getStateForTransition(std::string str);
        bool isInTransition(std::string t1, std::string alpha);
        void addNewTransition(std::pair<State, Symbol> src, std::set<State> dst);
        void addNewRevTransition(std::pair<State, Symbol> src, std::set<State> dst);
    protected:
        SetStates states; // set of all states of BA
        SetStates initialStates; // set of all initial states of BA
        SetStates acceptingStates; // set of all accepting states of BA
        SetSymbols alphabet; // set of all symbols of BA
        Delta<State, Symbol> transitions; // map that holds all the transitions of BA
        Delta<State, Symbol> reversedTransitions; // map that holds all the reversed transitions of BA
};

/**
 * Adds a new transition
 * @tparam State
 * @tparam Symbol
 * @param src
 * @param dst
 */
template<typename State, typename Symbol>
void Automaton<State, Symbol>::addNewTransition(std::pair<State, Symbol> src, std::set<State> dst){
    this->transitions.insert({src, dst});
}

/**
 * Adds a new reversed transition
 * @tparam State
 * @tparam Symbol
 * @param src
 * @param dst
 */
template<typename State, typename Symbol>
void Automaton<State, Symbol>::addNewRevTransition(std::pair<State, Symbol> src, std::set<State> dst){
    this->reversedTransitions.insert({src, dst});
}

#endif // AUTOMATON_H