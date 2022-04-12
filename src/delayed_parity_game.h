#ifndef DELAYED_PARITY_GAME_H
#define DELAYED_PARITY_GAME_H

template<typename State, typename Symbol> using Vertex0 = std::set<std::tuple<State, State, Symbol, int>>;
template<typename State> using Vertex1 = std::set<std::tuple<State, State, int>>;

template<typename State, typename Symbol>
class delayedParityGame : protected Automaton<State, Symbol> {
    public:
        void fun();
    protected:
        typedef struct DelayedV0 {
            Vertex0 v0;
            bool bit;
        }VD0;

        typedef struct DelayedV1 {
            Vertex1 v1;
            bool bit;
        }VD1;
};

template<typename State, typename Symbol>
void delayedParityGame<State, Symbol>::fun() {
    VD0 m_VD0;
}

#endif