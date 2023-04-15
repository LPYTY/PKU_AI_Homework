#include "parser.h"

Parser::Parser(int argc, char** argv)
	:argCount{ argc }, argValue{ argv }
{
}

AI* Parser::doParse()
{
	using namespace std::literals;
	for (int i = 1; i < argCount; i++)
	{
		if (argValue[i][0] != '-')
			error("Too many arguments"s, -2);
		i++;
		if (i >= argCount || argValue[i][0] == ' ')
			error("Too few arguments"s, -3);
		argTable[argValue[i - 1]] = argValue[i];
	}
	if (argTable.size() > 2)
		error("Too many commands"s, -4);
	bool outputFileSet = false;
	string fileName;
	if (argTable.find("-o") != argTable.end())
	{
		outputFileSet = true;
		fileName = argTable["-o"];
	}
	if (argTable.find("--output") != argTable.end())
	{
		outputFileSet = true;
		fileName = argTable["--output"];
	}
	if (argTable.find("--chat"s) != argTable.end())
	{
		if (!outputFileSet)
		{
			if (argTable.size() > 1)
				error("Too many commands"s, -4);
			return new ChatAI(myToken, argTable["--chat"s].c_str());
		}
		else
			return new ChatAI(myToken, argTable["--chat"s].c_str(), fileName.c_str());
	}
	else if (argTable.find("--draw"s) != argTable.end())
	{
		if (!outputFileSet)
			error("Output file not set"s, -5);
		return new DrawAI(myToken, argTable["--draw"s].c_str(), fileName.c_str());
	}
	else if (argTable.find("--math"s) != argTable.end())
	{
		if (!outputFileSet)
			error("Output file not set"s, -5);
		return new MathAI(myToken, argTable["--math"s].c_str(), fileName.c_str());
	}
	else
		error("Command expected"s, -6);
}

void Parser::error(const string& errorMsg, int exitCode)
{
	fprintf(stderr, errorMsg.c_str());
	fflush(stderr);
	exit(exitCode);
}

AI* Parser::parse(int argc, char** argv)
{
	Parser parser(argc, argv);
	return parser.doParse();
}
