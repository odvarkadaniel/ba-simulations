#include<stdlib.h>
#include<stdarg.h>
#include<iostream>
#include <iostream>
#include <fstream>

#include"util.h"
#include"automaton.h"

std::string printAutAsDot(Automaton<std::string, std::string> &omega,
                   std::set<std::pair<std::string, std::string>> &result) {
    std::string aut = "digraph finite_state_machine {\n"
	"fontname=\"Helvetica,Arial,sans-serif\"\n"
	"node [fontname=\"Helvetica,Arial,sans-serif\"]\n"
	"edge [fontname=\"Helvetica,Arial,sans-serif\"]\n"
	"rankdir=LR;\n";

    auto transitions = omega.getTransitions();
    auto acceptingStates = omega.getAcceptingStates();
    auto states = omega.getStates();

    aut += "node [shape = doublecircle]; ";
    for(const auto &acc : acceptingStates) {
        aut += acc;
        aut += " ";
    }
    aut += ";\n";

    aut += "node [shape = circle];\n";

    bool simulates = false;

    for(const auto &tr : transitions) {
        simulates = false;
        for(const auto &sec : tr.second) {
            for(const auto &res : result) {
                if(res.first == tr.first.first && res.second == sec) { //red color to signal that (s0, s1) are in relation
                    aut += tr.first.first;
                    aut += " -> ";
                    aut += sec;
                    aut += " [label = \"";
                    aut += tr.first.second;
                    aut += "\", ";
                    aut += "color = red];\n";
                    simulates = true;
                    break;
                }
            }
            if(!simulates) {
                aut += tr.first.first;
                aut += " -> ";
                aut += sec;
                aut += " [label = \"";
                aut += tr.first.second;
                aut += "\"];\n";
            }
        }
    }

    aut += "}";

    std::ofstream dotFile;
    dotFile.open("result.dot", std::ios_base::out);
    dotFile << aut;
    dotFile.close();
    
    return aut;
}

/**
 * @brief Prints usage of the program
 * 
 */
void printHelp() {
    using namespace std;

    cout << "Usage example:\n";
    cout << "  ./main --file=omega.ba --fair --fast --print\n\n";
    cout << "Usage:\n";
    cout << "  ./main --file=filename.ba { --fair | --direct | --delayed } { --fast } [dotfile.out]\n";
    cout << "Commands:\n";
    cout << "  { file / s } \t Set an input file ( supported formats are .BA, ...)\n";
    cout << "  { fair | direct | delayed } \t Set a simulation you want to compute\n";
    cout << "  [ fast ] \t Set the fast algorithm for fair or delayed simulation\n";
    cout << "  [ print ] \t Print the result relation into the terminal\n"; 
    cout << "  [ dot ] \t Outputs the automaton in a DOT format into file named omega.out\n";
    cout << "  [ help / h ] \t Prints help\n";
}

/**
 * Exits with appropriate message
 * @param fmt
 * @param ...
 */
void error_exit(const char *fmt, ...)
{
	va_list(args);
	fprintf(stderr, "ERROR: ");
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}

/**
 * Splits a vector with a specified delimiter
 * @param original
 * @param delimiter
 * @return
 */
std::vector<std::string> split(std::string original, std::string delimiter) {
    using namespace std;
    
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = original.find (delimiter, pos_start)) != string::npos) {
        token = original.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (original.substr (pos_start));
    return res;
}