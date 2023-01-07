// ========================================================
//
// FILE:			/source/rex/core/args.cpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Args implementation
//
// LAST EDITED:		January 2nd, 2023
//
// ========================================================

// Rex Engine private header
#include "rex_priv.hpp"

//
//
// Rex namespace: Args
//
//

namespace Rex
{

//
//
// Functions
//
//

// Parse arguments given on the command line
bool ParseCmdArgs(int argc, char **argv)
{
	if (argc < 1) return false;

	Arg arg;
	char buffer[256];
	rex_int i;

	for (i = 0; i < argc; i++)
	{
		// clear string buffer
		memset(buffer, 0, 256);

		// arg type
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			memcpy(buffer, argv[i] + 2, strlen(argv[i]) - 2);
		}
		else if (argv[i][0] == '-')
		{
			memcpy(buffer, argv[i] + 1, strlen(argv[i]) - 1);
		}
		else
		{
			memcpy(buffer, argv[i], strlen(argv[i]));
		}

		cout << buffer << endl;
	}

	return true;
}

//
// Args class
//

// Add an argument to the array
void Args::AddArg(Arg arg)
{
	args.push_back(arg);
}

// Get an argument from the array
Arg *Args::GetArg(rex_string cmd)
{
	for (rex_int i = 0; i < args.size(); i++)
	{
		if (args[i].cmd == cmd)
		{
			return &args[i];
		}
	}

	return nullptr;
}

} // namespace Rex
