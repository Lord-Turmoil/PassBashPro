/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : ExecFactory.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Factory class to get executables.                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../inc/exec/ExecFactory.h"

int ExecFactory::Hook(const char* descr, Exec exec)
{
    if (!descr)
        return -1;

    auto ret = m_records.emplace(descr, exec);
    if (ret.second)
        return 0;
    return 1;
    // already exists
}

// If descr is nullptr, all contents will be cleared.
int ExecFactory::UnHook(const char* descr, Exec exec)
{
    if (!descr)
    {
        m_records.clear();
        return 0;
    }

    // returns number of elements removed
    if (m_records.erase(descr))
        return 0;
    return -1;
}

Exec ExecFactory::Get(const char* descr)
{
    if (!descr)
        return nullptr;

    auto ret = m_records.find(descr);
    if (ret == m_records.end()) // no such executable.
        return nullptr;

    return ret->second;
}
