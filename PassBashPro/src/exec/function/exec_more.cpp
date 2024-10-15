/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_more.cpp                             *
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

#include "../../../inc/exec/function/FuncHeader.h"

static char _cmd[] = "cat";
static char _param[] = "-m";

static int _more_usage();

int exec_more(int argc, char* argv[])
{
    std::string path;

    if (_ParseArgs(argc, argv, path) != 0)
    {
        _more_usage();
        return 1;
    }

    XMLElementPtr node = PashDocUtil::GetNodeByPath(path);
    if (!node)
    {
        EXEC_PRINT_ERR("Password item doesn't exist!\n");
        return 2;
    }
    if (!PashDocUtil::IsItem(node))
    {
        EXEC_PRINT_ERR("You can only display a password item!\n");
        _more_usage();
        return 3;
    }

    _ShowItem(node, true);

    return 0;
}

static int _more_usage()
{
    return ExecHost::GetInstance()->execl(EXEC_GLOBAL, "help", "help", "more", nullptr);
}
