#ifndef PARSER_H
#define PARSER_H
#include "./ai.h"
#include "./ai_impl.h"
#include "./error.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <optional>

using std::unordered_map;
using std::string;
using std::vector;
using std::optional;

enum Command
{
    CHAT,
    DRAW,
    MATH,
    OUTPUT
};

class Parser 
{
    vector<string> args;
    size_t parsedCount;

    static unordered_map<string, Command> commandTable;

    static bool isCommand(const string& str);

    bool nextArgExists();
    void assertNextArgExists();
    Command parseCommand(const vector<Command>& expectedCommands);
    string parseString();

    Parser(int argc, char** argv);
    AI* doParse();

public:
    static AI* parse(int argc, char** argv);
};

#endif
