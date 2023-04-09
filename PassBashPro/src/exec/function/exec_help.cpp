/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_help.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   None                                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/ExecHeader.h"

static int _help_usage();
static int _help_parse_args(int argc, char* argv[], std::string& target);

int help(int argc, char* argv[])
{
	std::string target;
	if (_help_parse_args(argc, argv, target) != 0)
	{
		// Prevent potential recursive call.
		if (argc > 0 && argv[0][0] != '_')
			_help_usage();
	}

	std::string base = HELP_DIR;

}

static int _help_usage()
{
	// Use '_help' to differentiate internal and external call.
	return ExecHost::GetInstance()->execl(EXEC_GLOBAL, "help", "_help", "help", nullptr);
}

static int _help_parse_args(int argc, char* argv[], std::string& target)
{
	return _ParseArgs(argc, argv, target);
}
