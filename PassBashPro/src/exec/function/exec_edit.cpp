/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_edit.cpp                             *
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

static int _edit_usage();
static int _edit_parse_args(int argc, char* argv[], std::string& path);

int exec_edit(int argc, char* argv[])
{
    std::string path;

    if (_edit_parse_args(argc, argv, path) != 0)
    {
        _edit_usage();
        return 1;
    }

    XMLElementPtr node = PashDocUtil::GetNodeByPath(path);
    if (!node)
    {
        EXEC_PRINT_ERR("Password item doesn't exist!\n");
        node = PashDocUtil::CreateItemNodeByPath(path);
        cnsl::InsertText(MESSAGE_COLOR, "Password item \"%s\" created.\n", PashDocUtil::GetNodeDirectory(node, path));
    }
    else if (!PashDocUtil::IsItem(node))
    {
        EXEC_PRINT_ERR("You can only edit a password item!\n");
        _edit_usage();
        return 2;
    }

    int ret = ExecHost::GetInstance()->execl(EXEC_SERVICE, "editor", "editor", path.c_str(), nullptr);
    if (ret != 0)
    {
        EXEC_PRINT_ERR("Failed to launch password editor!\n");
        return 3;
    }

    g_doc.Mark();

    return 0;
}

static int _edit_usage()
{
    return ExecHost::GetInstance()->execl(MODE_TO_EXEC[g_mode], "help", "help", "edit", nullptr);
}

static int _edit_parse_args(int argc, char* argv[], std::string& path)
{
    return _ParseArgs(argc, argv, path);
}
