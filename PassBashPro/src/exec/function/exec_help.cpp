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

#include "../../../inc/exec/function/FuncHeader.h"

static int _help_usage();
static int _help_parse_args(int argc, char* argv[]);

static int _help_single();	// help single command (in detail)
static int _help_current();	// help current faction (in brief)
static int _help_all();		// help all factions (in brief)

static std::string& target;
static bool showAll;

int help(int argc, char* argv[])
{
	target = "";
	showAll = false;

	if (_help_parse_args(argc, argv) != 0)
	{
		// Prevent potential recursive call.
		if (argc > 0 && argv[0][0] != '_')
			_help_usage();
	}

	if (showAll)
		return _help_all();
	if (target == "")
		return _help_current();
	else
		return _help_single();
}

static int _help_usage()
{
	// Use '_help' to differentiate internal and external call.
	return ExecHost::GetInstance()->execl(EXEC_GLOBAL, "help", "_help", "help", nullptr);
}

static int _help_parse_args(int argc, char* argv[])
{
	int opt;
	int arg_cnt = 0;
	int opt_cnt = 0;
	bool err = false;
	while (opt = getopt(argc, argv, "a"))
	{
		switch (opt)
		{
		case 'a':
			showAll = true;
			break;
		case '!':
			arg_cnt++;
			if (arg_cnt == 1)
				target = optarg;
			else if (arg_cnt == 2)
			{
				err = true;
				EXEC_PRINT_ERR(ERRMSG_TOO_MANY "\n");
			}
			break;
		case '?':
			opt_cnt++;
			err = true;
			if (opt_cnt == 1)
				EXEC_PRINT_ERR("Unknown parameter \"-%c\"\n", optopt);
			break;
		default:
			break;
		}
	}

	if (err)
	{
		EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
		return 1;
	}

	return 0;
}
