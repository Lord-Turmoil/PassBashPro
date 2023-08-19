/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_cd.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
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

#include "../../../inc/exec/function/FuncHeader.h"

static XMLElementPtr last_dir = nullptr;

static int _cd_usage()
{
    return ExecHost::GetInstance()
        ->execl(EXEC_GLOBAL, "help", "help", "cd", nullptr);
}

static int _cd_parse_arg(int argc, char* argv[], std::string& path)
{
    return _ParseArgs(argc, argv, path);
}

int exec_cd(int argc, char* argv[])
{
    std::string path(SELF_DIR_NAME);

    if (_cd_parse_arg(argc, argv, path) != 0)
    {
        _cd_usage();
        return 1;
    }

    XMLElementPtr node;
    if (path == "-")
        node = last_dir;
    else
        node = PashDocUtil::GetNodeByPath(path);

    if (!node)
    {
        EXEC_PRINT_ERR("Group doesn't exist!\n");
        return false;
    }
    if (!PashDocUtil::IsGroup(node))
    {
        EXEC_PRINT_ERR("You can only enter a group!\n");
        _cd_usage();
        return false;
    }

    last_dir = g_doc.SetCurrent(node);
    PashDocUtil::GetPresentWorkingDirectory(g_pwd);

    return 0;
}
