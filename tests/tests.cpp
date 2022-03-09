#include<gtest/gtest.h>

#include"../src/simulations.h"

/**
 * @brief Construct a new TEST object for direct simulation relation test
 * 
 */
TEST(DirectSimulationRelation, DirectSimulation) {
    using namespace std;

    Automaton<std::string, std::string> a;
    a = a.loadAutomaton("tests/files/test1.ba");
    Simulation<std::string, std::string> s;
    std::set<std::pair<std::string, std::string>> actualOmega = s.directSimulationRelation(a);
    std::set<std::pair<std::string, std::string>> expectedOmega;

    expectedOmega.insert({make_pair("1", "2")});
    expectedOmega.insert({make_pair("1", "3")});
    expectedOmega.insert({make_pair("1", "4")});
    expectedOmega.insert({make_pair("2", "1")});
    expectedOmega.insert({make_pair("2", "3")});
    expectedOmega.insert({make_pair("2", "4")});
    expectedOmega.insert({make_pair("3", "1")});
    expectedOmega.insert({make_pair("3", "2")});
    expectedOmega.insert({make_pair("3", "4")});
    expectedOmega.insert({make_pair("4", "1")});
    expectedOmega.insert({make_pair("4", "2")});
    expectedOmega.insert({make_pair("4", "3")});

    EXPECT_EQ(expectedOmega, actualOmega);
}

TEST(loadAutomaton, load) {
    Automaton<std::string, std::string> actualAutomaton;
    actualAutomaton = actualAutomaton.loadAutomaton("tests/files/test0.ba");

    Automaton<std::string, std::string> expectedAutomaton;
    expectedAutomaton.addToAlphabet("a0");
    expectedAutomaton.addToAlphabet("a1");
    expectedAutomaton.addInitialState("[22]");
    expectedAutomaton.addAcceptingState("[3]");
    expectedAutomaton.addAcceptingState("[44]");
    expectedAutomaton.addNewTransition(std::make_pair("22", "a0"), std::set<std::string>{"3"});
    expectedAutomaton.addNewTransition(std::make_pair("3", "a1"), std::set<std::string>{"22"});
    expectedAutomaton.addNewRevTransition(std::make_pair("22", "a1"), std::set<std::string>{"3"});
    expectedAutomaton.addNewRevTransition(std::make_pair("3", "a0"), std::set<std::string>{"22"});

    EXPECT_EQ(expectedAutomaton.getAlphabet(), actualAutomaton.getAlphabet());
    EXPECT_EQ(expectedAutomaton.getInitialStates(), actualAutomaton.getInitialStates());
    EXPECT_EQ(expectedAutomaton.getAcceptingStates(), actualAutomaton.getAcceptingStates());
    EXPECT_EQ(expectedAutomaton.getTransitions(), actualAutomaton.getTransitions());
    EXPECT_EQ(expectedAutomaton.getReversedTransitions(), actualAutomaton.getReversedTransitions());
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}