// ========================================================
//
// FILE:			/source/rex/core/args.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL v1.4
//
// DESCRIPTION:		Rex namespace: Args header
//
// LAST EDITED:		January 2nd, 2023
//
// ========================================================

#ifndef __REX_ARGS_H__
#define __REX_ARGS_H__

//
//
// Rex namespace: Args
//
//

namespace Rex
{

//
//
// Classes
//
//

// Command line argument class
class Arg
{
	public:
		rex_string cmd;
		rex_string var;
};

// Command line argument array class
class Args
{
	public:
		void AddArg(Arg arg);
		Arg *GetArg(rex_string cmd);

	private:
		vector<Arg> args;
};

//
//
// Functions
//
//

// Parse arguments given on the command line
bool ParseCmdArgs(int argc, char **argv);

} // namespace Rex

#endif // __REX_ARGS_H__
