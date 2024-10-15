/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_archive.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 19, 2023                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Archive file into git repository.                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"

int exec_archive(int argc, char* argv[])
{
    return ExecHost::GetInstance()->execv(EXEC_SERVICE, "archive", argv);
}
