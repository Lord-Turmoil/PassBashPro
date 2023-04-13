/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : src_checkout.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 13, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Switch current user.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"

static std::string username;
static std::string password;

static int _checkout_usage();
static int _checkout_parse_args(int argc, char* argv[]);

int srv_checkout(int argc, char* argv[])
{
	username = "";
	password = "";

	if (_checkout_parse_args(argc, argv) != 0)
	{
		_checkout_usage();
		return 1;
	}

	if (!VerifyUsername(username))
		return 2;
	if (!VerifyPassword(password))
		return 3;

	EnvPtr env = CreateEnv(CreateProfile(username));
	VerifyProfileInit(env);

	if (!VerifyProfile(password.c_str()))
	{
		
	}
}

static int _checkout_usage()
{
	return ExecHost::GetInstance()
		->execl(EXEC_GLOBAL, "help", "help", "checkout", nullptr);
}

static int _checkout_parse_args(int argc, char* argv[])
{
	int opt;
	int arg_cnt = 0;
	int opt_cnt = 0;
	bool err = false;
	while (opt = getopt(argc, argv, "u:p:"))
	{
		if (opterr != 0)
		{
			EXEC_PRINT_ERR("Argument error: %s\n", optmsg);
			err = true;
			resetopt();
			break;
		}

		switch (opt)
		{
		case 'u':
			username = optarg;
			break;
		case 'p':
			password = optarg;
			break;
		case '!':
			arg_cnt++;
			if (arg_cnt == 1)
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

	if (username.empty() || password.empty())
	{
		EXEC_PRINT_ERR(ERRMSG_TOO_FEW "\n");
		EXEC_PRINT_MSG("Missing username or password.\n");
		err = true;
	}

	if (err)
	{
		EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
		return 1;
	}

	return 0;
}
