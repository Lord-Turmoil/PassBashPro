/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_order.cpp                            *
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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"

static int _order_parse_args(int argc, char* argv[], std::string& order);

int exec_order(int argc, char* argv[])
{
    std::string order;

    if (_order_parse_args(argc, argv, order) != 0)
    {
        // order doesn't have help info!
        return 1;
    }

    if (order == "msg")
        LOG_PRINT_MESSAGE();
    else if (order == "cmsg")
        LOG_CLEAR_MESSAGES();
    else if (order == "err")
        LOG_PRINT_ERROR();
    else if (order == "cerr")
        LOG_CLEAR_ERRORS();
    else if (order == "66")
    {
        if (g_doc.DebugSave(g_env))
            EXEC_PRINT_MSG("Password exported in plain text.\n");
        else
        {
            EXEC_PRINT_ERR("Failed to export password.\n");
            return 2;
        }
    }
    else if (order == "99")
    {
        if (g_doc.DebugLoad(g_env))
        {
            EXEC_PRINT_MSG("Plain text password imported.\n");
            g_doc.Mark();
        }
        else
        {
            EXEC_PRINT_ERR("Failed to import password.\n");
            return 3;
        }
    }
    else
    {
        EXEC_PRINT_ERR("Invalid order.\n");
        return 4;
    }

    return 0;
}

static int _order_parse_args(int argc, char* argv[], std::string& order)
{
    return _ParseArgs(argc, argv, order);
}
