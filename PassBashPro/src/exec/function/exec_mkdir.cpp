/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_mkdir.cpp                            *
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

static int _mkdir_usage();
static int _mkdir_parse_arg(int argc, char* argv[], std::string& path);

int exec_mkdir(int argc, char* argv[])
{
    std::string path;

    if (_mkdir_parse_arg(argc, argv, path) != 0)
    {
        _mkdir_usage();
        return 1;
    }

    XMLElementPtr node = PashDocUtil::GetNodeByPath(path);
    if (node)
    {
        EXEC_PRINT_ERR("%s with name \"%s\" already exists!\n",
                       PashDocUtil::IsGroup(node) ? "Group" : "Password item",
                       path.c_str());
        return 2;
    }

    node = PashDocUtil::CreateGroupNodeByPath(path);
    if (node)
        EXEC_PRINT_MSG("Group \"%s\" created.\n", path.c_str());
    else
    {
        EXEC_PRINT_ERR("Failed to create group \"%s\".\n", path.c_str());
        return 3;
    }

    g_doc.Mark();

    return 0;
}

static int _mkdir_parse_arg(int argc, char* argv[], std::string& path)
{
    return _ParseArgs(argc, argv, path);
}

static int _mkdir_usage()
{
    return ExecHost::GetInstance()
        ->execl(EXEC_GLOBAL, "help", "help", "mkdir", nullptr);
}
