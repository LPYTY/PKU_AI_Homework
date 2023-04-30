#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
#include <cstdlib>

using std::string;
using std::cerr;
using std::exit;

constexpr int COMMAND_EXPECTED = -1;
constexpr int UNKNOWN_COMMAND = -2;
constexpr int UNEXPECTED_COMMAND = -3;
constexpr int TOO_MANY_COMMAND = -4;
constexpr int TOO_FEW_ARGUMENTS = -5;
constexpr int INVALID_ARGUMENT = -6;
constexpr int FILE_ERROR = -7;

void errorExit(const string& errorMsg, int exitCode);

#endif // !ERROR_H
