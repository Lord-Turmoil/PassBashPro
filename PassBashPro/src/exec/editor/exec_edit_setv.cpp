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

static void _setv_usage();

int exec_edit_setv(int argc, char* argv[])
{
	const char* params[2] = { nullptr, nullptr };
	int ret = _edit_parse_params(argv[1], 2, params);

	if (ret != 2)
	{
		_setv_usage();
		return 1;
	}

	return _set_value(params[0], params[1]);
}

static void _setv_usage()
{
	ExecHost::GetInstance()
		->execl(EXEC_EDIT, "help", "help", "setv", nullptr);
}