/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_edit_clear.cpp                       *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
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

#include "../../../inc/exec/editor/EditorHeader.h"

static int _set_usage();

int exec_edit_set(int argc, char* argv[])
{
	const char* params[3] = { nullptr, nullptr, "-1" };
	int ret = _edit_parse_params(argv[1], 3, params);

	if (ret < 2 || ret > 3)
	{
		_set_usage();
		return 1;
	}

	return _set_entry(params[0], params[1], params[2]);
}

static int _set_usage()
{
	ExecHost::GetInstance()
		->execl(EXEC_EDIT, "help", "help", "set", nullptr);
}