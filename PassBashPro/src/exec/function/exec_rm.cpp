/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_rm.cpp                               *
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


static bool recursive;
static bool force;

static void _remove_init();
static int _remove_usage();
static int _remove_parse_args(int argc, char* argv[]);
static int _remove_confirm(const char* prompt);
static int _remove_root();
static int _remove_current();

int exec_rm(int argc, char* argv[])
{
	std::string prompt;
	std::string path;

	_remove_init();

	if (_remove_parse_args(argc, argv) != 0)
	{
		_remove_usage();
		return 1;
	}

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
			continue;
		path = argv[i];

		XMLElementPtr node = PashDocUtil::GetNodeByPath(path);
		if (!node)
		{
			EXEC_PRINT_ERR("%s doesn't exists.\n", node->Name());
		}
		else if (node == g_doc.GetRoot())
			return _remove_root();
		else if (node == g_doc.GetCurrent())
			return _remove_current();

		if (!PashDocUtil::IsParent(g_doc.GetCurrent(), node))
		{
			EXEC_PRINT_ERR("Do not attempt to remove parent group!\n");
			continue;
		}

		PashDocUtil::GetNodeDirectory(node, path);

		if (!recursive && PashDocUtil::IsGroup(node) && !node->NoChildren())
		{
			EXEC_PRINT_ERR("Group %s is not empty! ", path.c_str());
			EXEC_PRINT_MSG("Use -r to delete it recursively.\n");
			continue;
		}

		prompt = "Delete \"";
		prompt += path;
		prompt += "\"? (Y/N) $ ";
		if (_remove_confirm(prompt.c_str()))
		{
			if (PashDocUtil::IsGroup(node))
			{
				EXEC_PRINT_MSG("Group \"%s\" and its contents deleted permanently!\n",
							   path.c_str());
			}
			else
			{
				EXEC_PRINT_MSG("Password item \"%s\" deleted permanently!\n",
							   path.c_str());
			}
			PashDocUtil::DeleteNode(node);
		}
		else
			EXEC_PRINT_MSG("Nothing changed.\n");
	}

	g_doc.Mark();

	return true;
}

static void _remove_init()
{
	recursive = false;
	force = false;
}

static int _remove_usage()
{
	return ExecHost::GetInstance()
		->execl(EXEC_GLOBAL, "help", "help", "rm");
}

static int _remove_parse_args(int argc, char* argv[])
{
	int opt;
	bool err = false;
	while (opt = getopt(argc, argv, "rf"))
	{
		switch (opt)
		{
		case 'r':
			recursive = true;
			break;
		case 'f':
			force = true;
			break;
		case '?':
			err = true;
			EXEC_PRINT_ERR("Unknown parameter \"-%c\"\n", optopt);
			break;
		default:
			break;
		}
	}
	if (err)
	{
		EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
		cnsl::InsertNewLine();
		return 1;
	}

	return 0;
}

static int _remove_confirm(const char* prompt)
{
	if (force)
		return 1;

	char buffer[4];

	cnsl::InsertText(ERROR_COLOR, prompt);
	cnsl::GetString(buffer, cnsl::InputOptions(1, 1));
	cnsl::InsertNewLine();

	return (tolower(buffer[0]) == 'y');
}

static int _remove_root()
{
	bool old_force = force;
	force = false;	// Must confirm if attempt to remove root.

	if (_remove_confirm("You're sure to delete ALL passwords? (Y/N) $ "))
	{
		g_doc.SetCurrent(g_doc.GetRoot());
		PashDocUtil::GetPresentWorkingDirectory(g_pwd);
		PashDocUtil::DeleteChildren(g_doc.GetRoot());
		EXEC_PRINT_MSG("All passwords cleared!\n");
	}
	else
		EXEC_PRINT_MSG("Nothing changed.\n");

	force = old_force;

	return 0;
}

static int _remove_current()
{
	if (_remove_confirm("You're sure to delete current group? (Y/N) $ "))
	{
		XMLElementPtr current = g_doc.GetCurrent();
		XMLElementPtr parent = PashDocUtil::GetParentNode(current);
		g_doc.SetCurrent(parent);
		PashDocUtil::GetPresentWorkingDirectory(g_pwd);
		PashDocUtil::DeleteNode(current);
		EXEC_PRINT_MSG("Current group removed!\n");
	}
	else
		EXEC_PRINT_MSG("Nothing changed.\n");

	return 0;
}