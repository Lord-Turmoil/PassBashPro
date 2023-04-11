/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_ls.cpp                               *
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


static int _ls_usage();
static int _ls_parse_arg(int argc, char* argv[], std::string& path);

int exec_ls(int argc, char* argv[])
{
	std::string path;
	PassDocUtil::GetPresentWorkingDirectory(path);

	if (_ls_parse_arg(argc, argv, path) != 0)
	{
		_ls_usage();
		return 1;
	}

	XMLElementPtr node = PassDocUtil::GetNodeByPath(path);
	if (!node)
	{
		EXEC_PRINT_ERR("Group doesn't exist!\n");
		return 2;
	}
	else if (!PassDocUtil::IsGroup(node))
	{
		EXEC_PRINT_ERR("You can only list a group.\n");
		_ls_usage();
		return 3;
	}

	XMLElementPtrList list;
	PassDocUtil::GetChildren(node, list);
	if (list.empty())
		EXEC_PRINT_MSG("Nothing...\n");
	else
	{
		int size = (int)list.size();
		EXEC_PRINT_MSG("Total %d groups and items:\n", size);
		for (auto it : list)
		{
			if (PassDocUtil::IsGroup(it))
			{
				cnsl::InsertText(GROUP_COLOR, "\t%s/\n",
								 PassDocUtil::GetNodeAttr(it, "name"));
			}
			else
			{
				cnsl::InsertText(ITEM_COLOR, "\t%s\n",
								 PassDocUtil::GetNodeAttr(it, "name"));
			}
		}
	}

	return 0;
}

static int _ls_parse_arg(int argc, char* argv[], std::string& path)
{
	return _ParseOptionalArgs(argc, argv, path);
}

static int _ls_usage()
{
	return ExecHost::GetInstance()
		->execl(EXEC_GLOBAL, "help", "help", "ls", nullptr);
}