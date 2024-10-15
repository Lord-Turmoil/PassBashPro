/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_echo.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 15, 2023                            *
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

int exec_echo(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
        cnsl::InsertText("%s ", argv[i]);

    cnsl::InsertNewLine();

    return 0;
}
