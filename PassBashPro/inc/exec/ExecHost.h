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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _EXEC_HOST_H_
#define _EXEC_HOST_H_

#include "ExecFactory.h"
#include "../template/Singleton.h"


class ExecHost : public Singleton<ExecHost>
{
    friend class Singleton<ExecHost>;

public:
    int Register(const char* descr, ExecFactoryPtr factory);
    int UnRegister(const char* descr);

    ExecFactoryPtr GetFactory(const char* descr);

    // ends with nullptr
    int execl(const char* descr, const char* cmd, ...);

    // ends with nullptr
    int execv(const char* descr, const char* cmd, char* argv[]);

private:
    ExecHost()
    {
    }

    ~ExecHost() override
    {
    }

    std::unordered_map<std::string, ExecFactoryPtr> m_pool;
};

#endif
