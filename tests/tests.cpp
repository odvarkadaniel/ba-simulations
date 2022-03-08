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

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}