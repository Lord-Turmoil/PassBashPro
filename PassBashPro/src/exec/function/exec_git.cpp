/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_git.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 19, 2023                            *
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

#include "../../../inc/exec/function/FuncHeader.h"

int exec_git(int argc, char* argv[])
{
    std::string pwd(g_pwd);

    int ret = ExecHost::GetInstance()->execv(EXEC_SERVICE, "git", argv);

    ExecHost::GetInstance()->execl(EXEC_GLOBAL, "cd", "cd", pwd.c_str(), nullptr);

    return ret;
}
