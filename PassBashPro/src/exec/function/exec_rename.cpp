/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_rename.cpp                           *
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

static int _rename_usage();
static int _rename_parse_args(int argc, char* argv[], std::string& src, std::string& dest);

int exec_rename(int argc, char* argv[])
{
    std::string srcPath;
    std::string destName;

    if (_rename_parse_args(argc, argv, srcPath, destName) != 0)
    {
        _rename_usage();
        return 1;
    }

    XMLElementPtr node = PashDocUtil::GetNodeByPath(srcPath);
    if (!node)
    {
        EXEC_PRINT_ERR("Node doesn't exist!\n");
        return 2;
    }
    if (node == g_doc.GetRoot())
    {
        EXEC_PRINT_ERR("You can not rename root!\n");
        return 3;
    }
    if (!PashDocUtil::IsLegalNodeName(destName))
    {
        EXEC_PRINT_ERR("Illegal node name! No '/' allowed.\n");
        return 4;
    }

    XMLElementPtr parent = PashDocUtil::GetParentNode(node);
    if (PashDocUtil::GetDirectChildNode(parent, destName.c_str()))
    {
        EXEC_PRINT_ERR("New name already exist!\n");
        return 5;
    }

    std::string oldPath;
    std::string newPath;
    PashDocUtil::GetNodeDirectory(node, oldPath);
    node->SetAttribute("name", destName.c_str());
    PashDocUtil::GetNodeDirectory(node, newPath);

    EXEC_PRINT_MSG("Renamed \"%s\" to \"%s\".\n",
                   oldPath.c_str(), newPath.c_str());

    g_doc.Mark();

    return 0;
}

static int _rename_usage()
{
    return ExecHost::GetInstance()
        ->execl(EXEC_GLOBAL, "help", "help", "rename", nullptr);
}

static int _rename_parse_args(int argc, char* argv[], std::string& src, std::string& dest)
{
    return _ParseArgs(argc, argv, src, dest);
}
