/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_touch.cpp                            *
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
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"

static int _touch_usage();
static int _touch_parse_arg(int argc, char* argv[], std::string& path);


int exec_touch(int argc, char* argv[])
{
    std::string path("");

    if (_touch_parse_arg(argc, argv, path) != 0)
    {
        _touch_usage();
        return 1;
    }

    std::string name;
    PashDocUtil::GetBaseName(path, name);
    if (name.empty())
    {
        EXEC_PRINT_ERR("No name? You must specify a name!\n");
        return 2;
    }

    XMLElementPtr node = PashDocUtil::GetNodeByPath(path);
    if (node)
    {
        EXEC_PRINT_ERR("%s with name \"%s\" already exists!\n",
                       PashDocUtil::IsGroup(node) ? "Group" : "Password item",
                       path.c_str());
        return 3;
    }

    node = PashDocUtil::CreateItemNodeByPath(path);
    if (node)
        EXEC_PRINT_MSG("Password item \"%s\" created.\n", path.c_str());
    else
    {
        EXEC_PRINT_ERR("Failed to create password item \"%s\".\n", path.c_str());
        return 4;
    }

    g_doc.Mark();

    return 0;
}


static int _touch_usage()
{
    return ExecHost::GetInstance()
            ->execl(EXEC_GLOBAL, "help", "help", "touch", nullptr);
}


static int _touch_parse_arg(int argc, char* argv[], std::string& path)
{
    return _ParseArgs(argc, argv, path);
}
