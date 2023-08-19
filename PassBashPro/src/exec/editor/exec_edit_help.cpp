/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_edit_help.cpp                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 14, 2023                            *
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

#include <cstring>

static int _argc;
static char* _argv[EXEC_ARG_SIZE];
static char _buffer[EXEC_BUFFER_SIZE];
static char _cmd[] = "help";

int exec_edit_help(int argc, char* argv[])
{
    if (argc != 2)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        return 1;
    }

    char* context = nullptr;
    char* token;
    strcpy_s(_buffer, argv[1]);
    _argc = 0;
    _argv[_argc++] = _cmd;
    token = strtok_s(_buffer, " ", &context);
    while (token)
    {
        _argv[_argc++] = token;
        token = strtok_s(nullptr, " ", &context);
    }
    _argv[_argc] = nullptr;

    return ExecHost::GetInstance()->execv(EXEC_GLOBAL, _cmd, _argv);
}
