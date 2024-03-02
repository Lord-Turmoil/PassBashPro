/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_import.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 16, 2023                            *
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

#include "../../../inc/exec/service/ServiceHeader.h"
#include "../../../inc/utility/Auxiliary.h"

int _import_usage();
int _import_reload();


int srv_import(int argc, char* argv[])
{
    if (argc != 2)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        _import_usage();
        return 1;
    }

    if (!_STR_SAME(g_env->password, argv[1]))
    {
        EXEC_PRINT_ERR("Incorrect password for profile '%s'.\n",
                       g_env->username.c_str());
        return 2;
    }

    int ret = VerifyData(g_env);
    if (ret == 1)
    {
        EXEC_PRINT_ERR("Cannot find data to import!\n");
        EXEC_PRINT_MSG("Please ensure the data file name is the same as your profile name, and placed under ");
        wchar_t _buffer[PASH_BUFFER_SIZE];
        if (_wgetcwd(_buffer, PASH_BUFFER_SIZE - 1))
        {
            const char* path = narrow(_buffer);
            if (path)
            {
                EXEC_PRINT_MSG("\n\t%s\n", path);
                if (ExecHost::GetInstance()->execl(EXEC_GLOBAL, "copy", "copy", path, nullptr) == 0)
                {
                    EXEC_PRINT_MSG("Input path copied to clipboard, you can check it out.\n");
                }
            }
            else
                EXEC_PRINT_ERR("Currently not support Unicode path name.\n");
        }
        else
            EXEC_PRINT_MSG("root directory of PassBash.\n");

        return 3;
    }

    if (ret == 2)
    {
        EXEC_PRINT_ERR("Data verification error!\n");
        EXEC_PRINT_MSG("Data crashed, or doesn't belong to the current profile.\n");
        EXEC_PRINT_ERR("Failed to import data!\n");

        return 4;
    }

    if (!FileUtil::CopyFileToNew(g_env->username.c_str(), g_env->dataPath.c_str()))
    {
        EXEC_PRINT_ERR("Failed to merge data!\n");
        return 5;
    }

    PASH_PANIC_ON(_import_reload());

    EXEC_PRINT_MSG("Data imported!\n");

    return 0;
}


int _import_usage()
{
    return ExecHost::GetInstance()
            ->execl(MODE_TO_EXEC[g_mode], "help", "help", "import", nullptr);
}


int _import_reload()
{
    if (!g_doc.Load(g_env))
        return 1;

    return 0;
}
