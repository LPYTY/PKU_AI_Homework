#include "error.h"

void errorExit(const string& errorMsg, int exitCode)
{
	cerr << errorMsg.c_str();
	exit(exitCode);
}