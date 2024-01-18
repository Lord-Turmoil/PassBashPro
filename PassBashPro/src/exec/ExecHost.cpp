/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : ExecHost.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Executor of all executables.                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../inc/exec/ExecHost.h"
#include "../../inc/common/Error.h"
#include "../../inc/common/Constants.h"
#include "../../inc/common/Logger.h"

#include <stdarg.h>


int ExecHost::Register(const char* descr, ExecFactoryPtr factory)
{
    if (!descr)
        return -1;

    auto ret = m_pool.emplace(descr, factory);
    if (ret.second)
        return 0;
    return 1;
}


// if descr is nullptr, all factories will be cleared
int ExecHost::UnRegister(const char* descr)
{
    if (!descr)
    {
        m_pool.clear();
        return 0;
    }

    if (m_pool.erase(descr))
        return 0;
    return -1;
}


ExecFactoryPtr ExecHost::GetFactory(const char* descr)
{
    if (!descr)
        return nullptr;

    auto ret = m_pool.find(descr);
    if (ret == m_pool.end())
        return nullptr;

    return ret->second;
}


int ExecHost::execl(const char* descr, const char* cmd, ...)
{
    static char* argv[EXEC_ARG_SIZE];

    auto factory = GetFactory(descr);
    if (!factory)
    {
        LOG_ERROR("Factory '%s' not found", descr);
        return ERRNO_EXEC_NOT_FOUND;
    }
    auto exec = factory->Get(cmd);
    if (!exec)
    {
        LOG_ERROR("Command '%s' not found", cmd);
        return ERRNO_EXEC_NOT_FOUND;
    }


    int argc = 0;
    char* arg;
    va_list args;

    va_start(args, cmd);
    arg = va_arg(args, char*);
    while (arg)
    {
        argv[argc++] = arg;
        arg = va_arg(args, char*);
    }
    argv[argc] = nullptr;

    return exec(argc, argv);
}


int ExecHost::execv(const char* descr, const char* cmd, char* argv[])
{
    auto factory = GetFactory(descr);
    if (!factory)
        return ERRNO_EXEC_NOT_FOUND;
    auto exec = factory->Get(cmd);
    if (!exec)
        return ERRNO_EXEC_NOT_FOUND;

    int argc = 0;
    char** arg = argv;
    while (*arg)
    {
        arg++;
        argc++;
    }

    return exec(argc, argv);
}
