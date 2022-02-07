#include<stdlib.h>
#include<stdarg.h>
#include<iostream>

#include"util.h"

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