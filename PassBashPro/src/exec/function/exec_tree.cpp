/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_tree.cpp                             *
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

static bool group_only;

static void _tree_init();
static int _tree_usage();
static int _tree_parse_args(int argc, char* argv[], std::string& path);
static void _tree(XMLElementPtr node, const std::string& leading);

int exec_tree(int argc, char* argv[])
{
	std::string path;

	_tree_init();

	PashDocUtil::GetPresentWorkingDirectory(path);
	if (_tree_parse_args(argc, argv, path) != 0)
	{
		_tree_usage();
		return 1;
	}

	XMLElementPtr node = PashDocUtil::GetNodeByPath(path);
	if (!node)
	{
		EXEC_PRINT_ERR("Group doesn't exist!\n");
		return 2;
	}

	_tree(node, " ");

	return 0;
}

static void _tree_init()
{
	group_only = false;
}

static int _tree_usage()
{
	return ExecHost::GetInstance()
		->execl(EXEC_GLOBAL, "help", "help", "tree", nullptr);
}

static int _tree_parse_args(int argc, char* argv[], std::string& path)
{
	int opt;
	int arg_cnt = 0;
	bool err = false;
	while (opt = getopt(argc, argv, "g"))
	{
		switch (opt)
		{
		case 'g':
			group_only = true;
			break;
		case '!':
			arg_cnt++;
			if (arg_cnt == 1)
				path = optarg;
			else if (arg_cnt == 2)
			{
				err = true;
				EXEC_PRINT_ERR(ERRMSG_TOO_MANY "\n");
			}
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
		return 1;
	}

	return 0;
}

/********************************************************************
** This is a little tricky.
** To make it clear, '.' is for leading space.
** . current [leading "."]
** ...|-- alpha [leading ".'..|.."]
** ...|..  \-- beta [leading ".'..|..'.....]
** ...
*/
static void _tree(XMLElementPtr node, const std::string& leading)
{
	// First output itself, with leading previously added.
	if (PashDocUtil::IsGroup(node))
		cnsl::InsertText(GROUP_COLOR, " %s/\n", PashDocUtil::GetNodeName(node));
	else
	{
		cnsl::InsertText(ITEM_COLOR, " %s\n", PashDocUtil::GetNodeName(node));
		return;
	}

	// Then, output its children with leading.
	XMLElementPtrList list;
	if (group_only)
		PashDocUtil::GetGroupChildren(node, list);
	else
		PashDocUtil::GetChildren(node, list);
	for (auto it : list)
	{
		cnsl::InsertText("%s", leading.c_str());
		if (it != list.back())
		{
			cnsl::InsertText("  |--");
			_tree(it, leading + "  |  ");
		}
		else
		{
			cnsl::InsertText("  \\--");
			_tree(it, leading + "     ");
		}
		it = it->NextSiblingElement();
	}
}