#ifndef PARSER_H
#define PARSER_H
#include "./ai.h"
#include "./ai_impl.h"

#include <unordered_map>
#include <string>

using std::unordered_map;

class Parser 
{
    int argCount;
    char** argValue;
    unordered_map<string, string> argTable;

    Parser(int argc, char** argv);
    AI* doParse();
    void error(const string& errorMsg, int exitCode);

public:
    static AI* parse(int argc, char** argv);
};

#endif
