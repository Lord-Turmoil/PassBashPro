/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_unknown.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 12, 2023                            *
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

int exec_unknown(int argc, char* argv[])
{
    if (argc == 2)
        EXEC_PRINT_ERR("\"%s\" is not a command.\n", argv[1]);
    else
        EXEC_PRINT_ERR("Command not recognized.\n");

    EXEC_PRINT_MSG("Use \"help\" for more information.\n");

    return 0;
}
