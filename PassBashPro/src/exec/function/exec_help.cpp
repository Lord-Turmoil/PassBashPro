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
#include "../../../inc/utility/FileUtil.h"

static std::string target;
static bool showAll;
static char _help_buffer[PASH_BUFFER_SIZE];

static int _help_usage();
static int _help_parse_args(int argc, char* argv[]);

static int _help_single(const std::string& item, bool brief = false);	// help single command (in detail)
static int _help_current(const char* root = nullptr);	// help current faction (in brief)
static int _help_all();		// help all factions (in brief)

static int _print_help(const char* path, int n = -1);

int exec_help(int argc, char* argv[])
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
		return _help_single(target);
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


static int _help_single(const std::string& item, bool brief)	// help single command (in detail)
{
	std::string path(HELP_ROOT_DIR);
	path.append(HELP_DIR[g_mode]);
	path.append(item);

	int ret = _print_help(path.c_str(), brief ? 1 : -1);
	if (ret != 0)
	{
		EXEC_PRINT_ERR("Help for '%s' is not available!\n", item.c_str());
		return 1;
	}

	return 0;
}

static int _help_current(const char* root)	// help current faction (in brief)
{
	std::string path;
	
	if (root)
		path.assign(root);
	else
		path.assign(HELP_ROOT_DIR).append(HELP_DIR[g_mode]);

	std::vector<std::string> names;
	FileUtil::GetFiles(path.c_str(), nullptr, &names);

	if (names.empty())
		cnsl::InsertText("No available help information.\n");
	else
	{
		for (auto name : names)
			_help_single(name, true);
	}

	return 0;
}

static int _help_all()		// help all factions (in brief)
{
	std::string path(HELP_ROOT_DIR);
	std::vector<std::string> dirs;
	int ret = 0;

	FileUtil::GetDirectories(path.c_str(), &dirs, nullptr);
	if (dirs.empty())
		cnsl::InsertText("No available help information.\n");
	else
	{
		std::string faction;
		for (auto dir : dirs)
		{
			if (_help_current(dir.c_str()) != 0)
				ret++;
		}
	}

	return ret;
}

static int _print_help(const char* path, int n)
{
	FILE* fp;

	if (fopen_s(&fp, path, "r") != 0)
		return 1;

	WORD old = cnsl::SetTextForeground(MESSAGE_COLOR);

	if (n < 0)
		n = 1024;
	while ((n > 0) && (fgets(_help_buffer, PASH_BUFFER_SIZE, fp) != NULL))
	{
		n--;
		cnsl::InsertText("%s", _help_buffer);
	}
	
	cnsl::SetTextForeground(old);

	fclose(fp);

	return 0;
}
