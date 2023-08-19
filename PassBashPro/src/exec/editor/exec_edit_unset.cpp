/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_edit_clear.cpp                       *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
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

#include "../../../inc/exec/editor/EditorHeader.h"

static void _unset_usage();

int exec_edit_unset(int argc, char* argv[])
{
    const char* params[1] = {nullptr};
    int ret = _edit_parse_params(argv[1], 1, params);

    if (ret != 1)
    {
        _unset_usage();
        return 1;
    }

    int id;
    if (!tinyxml2::XMLUtil::ToInt(params[0], &id))
    {
        cnsl::InsertText("Invalid ID format!\n");
        return 2;
    }
    XMLElementPtr entry = PashDocUtil::GetEntryNode(_edit_item, id);
    if (!entry)
    {
        cnsl::InsertText(ERROR_COLOR, "Entry with ID \"%d\" doesn't exist!\n", id);
        return 3;
    }

    _ShowItemSimple(_edit_item, false, entry->Attribute("key"), ENTRY_DELETE_COLOR);
    PashDocUtil::DeleteNode(entry);

    return 0;
}

static void _unset_usage()
{
    ExecHost::GetInstance()
        ->execl(EXEC_EDIT, "help", "help", "unset", nullptr);
}
