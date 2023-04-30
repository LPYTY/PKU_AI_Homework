#include "./parser.h"

bool Parser::isCommand(const string& str)
{
	return str.size() >= 2 && str[0] == '-';
}

Parser::Parser(int argc, char** argv)
	:parsedCount{ 0 }
{
	for (int i = 1; i < argc; i++)
		args.push_back(argv[i]);
}

AI* Parser::doParse()
{
	parsedCount = 0;
	using namespace std::literals;
	optional<Command> AIType;
	string stringToSend;
	optional<string> fileName;
	while(nextArgExists())
	{
		vector<Command> expectedCommands;
		if (!AIType)
		{
			expectedCommands = { Command::CHAT, Command::DRAW, Command::MATH };
		}
		if (!fileName)
		{
			expectedCommands.push_back(Command::OUTPUT);
		}
		Command currentCommand = parseCommand(expectedCommands);
		switch (currentCommand)
		{
		case Command::CHAT:
		case Command::DRAW:
		case Command::MATH:
			if (AIType)
				errorExit("Too many commands", TOO_MANY_COMMAND);
			stringToSend = parseString();
			AIType = currentCommand;
			break;
		case Command::OUTPUT:
			if (fileName)
				errorExit("Too many commands", TOO_MANY_COMMAND);
			fileName = parseString();
			break;
		default:
			break;
		}
	}
	if (!AIType)
		errorExit("AI type not specified", TOO_FEW_ARGUMENTS);
	switch (*AIType)
	{
	case Command::CHAT:
	{
		ChatAI* ai = new ChatAI(myToken, stringToSend);
		if (fileName)
			ai->attachFile(fileName->c_str());
		return ai;
	}
	case Command::DRAW:
		if (!fileName)
			errorExit("Output file not specified", TOO_FEW_ARGUMENTS);
		return new DrawAI(myToken, stringToSend, *fileName);
	case Command::MATH:
		if (!fileName)
			errorExit("Output file not specified", TOO_FEW_ARGUMENTS);
		return new MathAI(myToken, stringToSend, *fileName);
	default:
		break;
	}
	return nullptr;
}

bool Parser::nextArgExists()
{
	return parsedCount < args.size();
}

void Parser::assertNextArgExists()
{
	using namespace std::literals;
	if (parsedCount >= args.size())
		errorExit("More arguments expected"s, TOO_FEW_ARGUMENTS);
}

Command Parser::parseCommand(const vector<Command>& expectedCommands)
{
	assertNextArgExists();
	using namespace std::literals;
	const string& cmd = args[parsedCount];
	parsedCount++;
	if (!isCommand(cmd))
	{
		errorExit("Command expected"s, COMMAND_EXPECTED);
	}
	auto iter = commandTable.find(cmd);
	if (iter == commandTable.end())
		errorExit("Unknown command"s, UNKNOWN_COMMAND);
	if (std::find(expectedCommands.begin(), expectedCommands.end(), iter->second) == expectedCommands.end())
		errorExit("Unexpected command"s, UNEXPECTED_COMMAND);
	return iter->second;
}

string Parser::parseString()
{
	assertNextArgExists();
	using namespace std::literals;
	const string& str = args[parsedCount];
	parsedCount++;
	if (isCommand(str))
		errorExit("Argument expected", TOO_FEW_ARGUMENTS);
	return str;
}

AI* Parser::parse(int argc, char** argv)
{
	Parser parser(argc, argv);
	return parser.doParse();
}

unordered_map<string, Command> Parser::commandTable =
{
	std::make_pair("--chat", Command::CHAT),
	std::make_pair("--draw", Command::DRAW),
	std::make_pair("--math", Command::MATH),
	std::make_pair("--output", Command::OUTPUT),
	std::make_pair("-o", Command::OUTPUT),
};