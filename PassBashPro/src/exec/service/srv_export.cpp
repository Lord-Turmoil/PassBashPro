/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_export.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 16, 2023                            *
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

#include "../../../inc/exec/service/ServiceHeader.h"


int _export_usage();
int srv_export(int argc, char* argv[])
{
	if (argc != 2)
	{
		EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
		_export_usage();
		return 1;
	}

	if (!_STR_SAME(g_env->password, argv[1]))
	{
		EXEC_PRINT_ERR("Incorrect password for profile '%s'.\n",
					   g_env->username.c_str());
		return 2;
	}

	int ret = ExecHost::GetInstance()->execl(EXEC_SERVICE, "save", "save", nullptr);
	if (ret != 0)
		return 3;

	if (!FileUtil::CopyFileToNew(g_env->dataPath.c_str(), g_env->username.c_str()))
	{
		EXEC_PRINT_ERR("Failed to export data!\n");
		return 4;
	}

	wchar_t _buffer[PASH_BUFFER_SIZE];
	if (_wgetcwd(_buffer, PASH_BUFFER_SIZE - 1) == nullptr)
	{
		EXEC_PRINT_ERR("Failed to get export path!\n");
		EXEC_PRINT_MSG("You can still find the exported data at root directory of PassBash.\n");
		return 5;
	}

	const char* path = narrow(_buffer);
	if (path)
	{
		EXEC_PRINT_MSG("Data exported to '%s'.\n", path);
		if (ExecHost::GetInstance()->execl(EXEC_GLOBAL, "copy", "copy", path, nullptr) == 0)
		{
			EXEC_PRINT_MSG("Output path copied to clipboard.\n");
		}
	}
	else
	{
		EXEC_PRINT_ERR("Currently not support Unicode path name.\n");
	}

	return 0;
}

int _export_usage()
{
	return ExecHost::GetInstance()
		->execl(MODE_TO_EXEC[g_mode], "help", "help", "export", nullptr);
}
