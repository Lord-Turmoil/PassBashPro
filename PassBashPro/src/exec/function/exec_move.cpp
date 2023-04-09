/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_move.cpp                             *
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

static int _move_usage();
static int _move_parse_args(int argc, char* argv[], std::string& src, std::string& dest);

int exec_move(int argc, char* argv[])
{
	std::string srcPath;
	std::string destPath;

	if (_move_parse_args(argc, argv, srcPath, destPath) != 0)
	{
		_move_usage();
		return 1;
	}

	XMLElementPtr srcNode = PassDocUtil::GetNodeByPath(srcPath);
	XMLElementPtr destGroup = PassDocUtil::GetNodeByPath(destPath);

	if (!srcNode)
	{
		EXEC_PRINT_ERR("Source doesn't exist!\n");
		return 2;
	}
	if (!destGroup)
	{
		EXEC_PRINT_ERR("Destination group doesn't exist!\n");
		return 3;
	}
	if (!PassDocUtil::IsGroup(destGroup))
	{
		EXEC_PRINT_ERR("Destination must be a group!\n");
		return 4;
	}
	if (srcNode == destGroup)
	{
		EXEC_PRINT_ERR("Source and destination must not be the same!\n");
		return 5;
	}

	/*
	** 2022/01/19 TS:
	** Fixed name duplicated bug.
	*/
	if (PassDocUtil::GetDirectChildNode(destGroup, PassDocUtil::GetNodeName(srcNode)))
	{
		EXEC_PRINT_ERR("Name exists in destination group!\n");
		return 6;
	}

	// See tinyxml2, it will automatically move from old place.
	std::string oldPath;
	std::string newPath;
	PassDocUtil::GetNodeDirectory(srcNode, oldPath);
	PassDocUtil::AddChildNode(destGroup, srcNode);
	PassDocUtil::GetNodeDirectory(srcNode, newPath);
	EXEC_PRINT_MSG("Moved \"%s\" to \"%s\".\n",
					 oldPath.c_str(), newPath.c_str());

	g_doc.Mark();

	return 0;
}

static int _move_usage()
{
	return ExecHost::GetInstance()
		->execl(EXEC_GLOBAL, "help", "help", "move", nullptr);
}

static int _move_parse_args(int argc, char* argv[], std::string& src, std::string& dest)
{
	return _ParseArgs(argc, argv, src, dest);
}
