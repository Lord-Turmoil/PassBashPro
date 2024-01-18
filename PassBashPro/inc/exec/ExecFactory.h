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

#pragma once

#ifndef _EXEC_FACTORY_H_
#define _EXEC_FACTORY_H_

#include "Exec.h"

#include <unordered_map>
#include <string>
#include <memory>


/********************************************************************
** ExecFactory only stores executables.
*/
class ExecFactory
{
public:
    int Hook(const char* descr, Exec exec);
    int UnHook(const char* descr, Exec exec);

    Exec Get(const char* descr);

private:
    std::unordered_map<std::string, Exec> m_records;
};


using ExecFactoryPtr = std::shared_ptr<ExecFactory>;


#endif
