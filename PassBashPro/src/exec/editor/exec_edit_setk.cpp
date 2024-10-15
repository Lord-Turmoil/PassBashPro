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
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../../inc/exec/editor/EditorHeader.h"

static void _setk_usage();

int exec_edit_setk(int argc, char* argv[])
{
    const char* params[2] = { nullptr, nullptr };
    int ret = _edit_parse_params(argv[1], 2, params);

    if (ret != 2)
    {
        _setk_usage();
        return 1;
    }

    return _set_key(params[0], params[1]);
}

static void _setk_usage()
{
    ExecHost::GetInstance()->execl(EXEC_EDIT, "help", "help", "setk", nullptr);
}
