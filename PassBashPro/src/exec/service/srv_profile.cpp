/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_profil.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Create local profile.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"
#include "../../../inc/core/Profile.h"


std::string username;
std::string email;
std::string password;

static bool useCli;

static void _profile_init();
static int _profile_usage();
static int _profile_parse_arg(int argc, char* argv[]);
static int _profile_cli();
static int _profile_silent();

static int _profile_receive_username();
static int _profile_receive_password();

int srv_profile(int argc, char* argv[])
{
	_profile_init();

	if (_profile_parse_arg(argc, argv) != 0)
	{
		_profile_usage();
		return 1;
	}

	if (useCli)
		return _profile_cli();
	else
		return _profile_silent();
}

static void _profile_init()
{
	username = "";
	email = "";
	password = "";

	useCli = false;
}

static int _profile_usage()
{
	return ExecHost::GetInstance()
		->execl(EXEC_GLOBAL, "help", "help", "profile", nullptr);
}

static int _profile_parse_arg(int argc, char* argv[])
{
	int opt;
	int arg_cnt = 0;
	int opt_cnt = 0;
	bool err = false;
	while (opt = getopt(argc, argv, "iu:p:"))
	{
		switch (opt)
		{
		case 'u':
			username = optarg;
			break;
		case 'p':
			username = optarg;
			break;
		case 'i':
			useCli = true;
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

	if (!useCli && username.empty() && password.empty())
	{
		EXEC_PRINT_ERR(ERRMSG_TOO_FEW "\n");
		err = true;
	}

	if (err)
	{
		EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
		return 1;
	}

	return 0;
}

static int _profile_cli()
{
	int ret = _profile_receive_username();
	if (ret == 0)
	{
		ret = _profile_receive_password();
		if (ret == 0)
			return _profile_silent();
	}
	
	return ret;
}


static int _profile_silent()
{
	ProfilePool* pool = ProfilePool::GetInstance();

	if (pool->Get(username))
	{
		EXEC_PRINT_ERR("Username '%s' already exists!\n", username.c_str());
		return 10;
	}
	if (password.length() < PASSWORD_MIN_LENGTH)
	{
		EXEC_PRINT_ERR("Password too short! 6 ~ 16 characters.\n");
		return 11;
	}
	if (password.length() > PASSWORD_MAX_LENGTH)
	{
		EXEC_PRINT_ERR("Password too long! 6 ~ 16 characters.\n");
		return 12;
	}

	std::string path(PASH_DIR);
	path.append(username).append("\\");
	
	// use physical directory to mark profile
	if (!FileUtil::NewDirectory(path.c_str()))
		return 13;
	
	Profile profile(username, path);
	pool->Add(profile);
	EnvPtr env = CreateEnv(&profile);
	_FormatPassword(password.c_str(), env->password);
	
	InitEnvFiles(env);

	return 0;
}

static int _profile_receive_username()
{
	char buffer[USERNAME_BUFFER_SIZE];
	ProfilePool* pool = ProfilePool::GetInstance();

	cnsl::InsertText("Please enter the ");
	cnsl::InsertText(HIGHLIGHT_COLOR, "username");
	cnsl::InsertText(" for this profile:\n");

	cnsl::InsertText(MESSAGE_COLOR,
					 "1 to %d characters, only a-zA-Z0-9 is allowed.\n",
					 USERNAME_MAX_LENGTH);
	cnsl::InsertText(PROMPT_COLOR, "$ ");

	cnsl::InputOptions options;
	options.minLen = 1;
	options.maxLen = USERNAME_MAX_LENGTH;
	options.interruptible = true;

	for (; ; )
	{
		int ret = cnsl::GetString(buffer, options);
		while (ret < options.minLen)
		{
			if (ret == -1)
				return -TERMINATION;
			cnsl::Clear(0);
			cnsl::InsertText(PROMPT_COLOR, "$ ");
			ret = cnsl::GetString(buffer, options);
		}
		strstrip(buffer);
		if (pool->Get(buffer))
		{
			cnsl::InsertNewLine();
			EXEC_PRINT_ERR("Username already exists!");
			Sleep(500);
			cnsl::Clear(0);
			cnsl::InsertReverseLineFeed();
			cnsl::Clear(0);
			cnsl::InsertText(PROMPT_COLOR, "$ ");
		}
		else
			break;
	}
	cnsl::InsertNewLine();

	username.assign(buffer);

	return 0;
}

static int _profile_receive_password()
{
	char buffer[PASSWORD_BUFFER_SIZE];

	cnsl::InsertText("Please enter the ");
	cnsl::InsertText(HIGHLIGHT_COLOR, "password");
	cnsl::InsertText(" for this profile:\n");

	cnsl::InsertText(MESSAGE_COLOR,
					 "%d to %d characters, any ascii that is printable.\n",
					 PASSWORD_MIN_LENGTH, PASSWORD_MAX_LENGTH);
	cnsl::InsertText(PROMPT_COLOR, "$ ");

	cnsl::InputOptions options;
	options.minLen = PASSWORD_MIN_LENGTH;
	options.maxLen = PASSWORD_MAX_LENGTH;
	options.interruptible = true;

	int ret = cnsl::GetString(buffer, options);
	while (ret < options.minLen)
	{
		if (ret == -1)
			return -TERMINATION;
		cnsl::Clear(0);
		cnsl::InsertText(PROMPT_COLOR, "$ ");
		ret = cnsl::GetString(buffer, options);
	}
	cnsl::InsertNewLine();
	_FormatPassword(buffer);

	password.assign(buffer);

	return 0;
}