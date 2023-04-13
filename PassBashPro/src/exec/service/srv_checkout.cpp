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
static int _checkout_logout();
static int _checkout_login(EnvPtr env, PashDoc& doc);

int srv_checkout(int argc, char* argv[])
{
	PASH_PANIC_ON(g_env == nullptr);

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

	ProfilePtr profile = ProfilePool::GetInstance()->Get(username);
	if (!profile)
	{
		EXEC_PRINT_ERR("User does not exist!\n");
		return 4;
	}

	EnvPtr env = CreateEnv(profile);
	VerifyProfileInit(env);
	if (!VerifyProfile(password.c_str()))
	{
		EXEC_PRINT_ERR("The password is incorrect.\n");
		return 5;
	}

	_FormatPassword(password.c_str(), env->password);
	PashDoc doc;

	if (_checkout_login(env, doc) != 0)
	{
		EXEC_PRINT_ERR("Failed to checkout.\n");
		EXEC_PRINT_MSG("Stay on current user.\n");
		return 6;
	}
	PASH_PANIC_ON(_checkout_logout());

	// switch user
	g_env = env;
	g_doc = doc;

	EXEC_PRINT_MSG("Successfully change profile to '%s'.\n", g_env->username.c_str());
	UpdateCache();

	return 0;
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

static int _checkout_logout()
{
	PASH_TRY(SaveConfig(g_env, true));
	PASH_TRY(SaveData(g_doc, g_env, true));

	return 0;
}

static int _checkout_login(EnvPtr env, PashDoc& doc)
{
	if (!doc.Load(env))
		return 1;

	return 0;
}
