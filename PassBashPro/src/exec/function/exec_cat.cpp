/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_cat.cpp                              *
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

static int _cat_usage()
{
	return ExecHost::GetInstance()
		->execl(EXEC_GLOBAL, "help", "help", "cat", nullptr);
}

int exec_cat(int argc, char* argv[])
{
	std::string path;

	if (_ParseArgs(argc, argv, path) != 0)
	{
		_cat_usage();
		return 1;
	}

	XMLElementPtr node = PassDocUtil::GetNodeByPath(path);
	if (!node)
	{
		EXEC_PRINT_ERR("Password item doesn't exist!\n");
		return 2;
	}
	if (!PassDocUtil::IsItem(node))
	{
		EXEC_PRINT_ERR("You can only cat a password item!\n");
		_cat_usage();
		return 3;
	}

	_ShowItem(node, nullptr);

	return 0;
}