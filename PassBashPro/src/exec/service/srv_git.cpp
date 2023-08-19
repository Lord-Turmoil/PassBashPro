/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_git.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 19, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Use external git.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"

#include <direct.h>
#include <string>
#include <cstdlib>

#include <tea.h>


int _git_verify();
int _git_reload();

int srv_git(int argc, char* argv[])
{
    PASH_PANIC_ON(g_env == nullptr);

    std::wstring curPath;
    std::wstring userPath;

    wchar_t cwd[PASH_BUFFER_SIZE];

    if (_wgetcwd(cwd, PASH_BUFFER_SIZE) == nullptr)
    {
        EXEC_PRINT_ERR("Cannot get working directory.\n");
        return 1;
    }
    curPath.assign(cwd);
    if (curPath.back() != L'\\')
        curPath.push_back(L'\\');
    userPath.assign(curPath);
    userPath.append(widen(PASH_DIR));
    userPath.append(widen(g_env->username.c_str()));
    userPath.append(L"\\");

    // Check git repository.
    if (argc >= 2)
    {
        std::wstring gitPath(userPath);
        gitPath.append(L".git\\");
        if (_STR_SAME(argv[1], "init"))
        {
            if (FileUtil::Exists(gitPath.c_str()))
            {
                EXEC_PRINT_ERR("Git repository already initialized!\n");
                return 2;
            }
        }
        else if (_STR_SAME(argv[1], "destroy"))
        {
            FileUtil::DeletePath(gitPath.c_str());
            EXEC_PRINT_MSG("Git repository deleted!\n");
            return 0;
        }
    }

    // Change working directory
    if (_wchdir(userPath.c_str()) != 0)
    {
        EXEC_PRINT_ERR("Failed to change directory.\n");
        return 2;
    }

    // Get git command.
    char cmd[EXEC_BUFFER_SIZE];
    tea::TEABufferWriter writer(cmd);
    for (int i = 0; i < argc; i++)
    {
        writer.Write(argv[i], strlen(argv[i]));
        writer.Write(" ", 1);
    }
    writer.Close();

    int ret = system(cmd);
    if (ret != 0)
    {
        EXEC_PRINT_ERR("Something wrong happened.\n");
        ret = 3;
    }

    // Change working directory back.
    if (_wchdir(curPath.c_str()) != 0)
    {
        PASH_PANIC("Failed to change directory back.");
        // Not return.
    }

    g_doc.UnLoad();

    // Verify config.
    if (_git_verify() != 0) // password change
    {
        int r = ExecHost::GetInstance()->execl(EXEC_SERVICE, "login", "login", nullptr);
        if (r != 0)
            return TERMINATION;
        // g_doc will be loaded at the end of successful login.
    }
    else
        g_doc.Load(g_env);
    _git_reload();

    return ret;
}

int _git_verify()
{
    VerifyProfileInit(g_env);
    return VerifyProfile(g_env->password) ? 0 : 1;
}

int _git_reload()
{
    // nothing...
    return 0;
}
