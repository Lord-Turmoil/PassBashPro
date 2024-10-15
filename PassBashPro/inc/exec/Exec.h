/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Exec.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Basic declarations of executables.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _EXEC_H_
#define _EXEC_H_

using Exec = int (*)(int, char*[]);

// Identifiers for different executable factories.
constexpr char EXEC_IDLE[] = "offline";
constexpr char EXEC_GLOBAL[] = "global";
constexpr char EXEC_EDIT[] = "editor";
constexpr char EXEC_HIDDEN[] = "hidden";
constexpr char EXEC_SERVICE[] = "service";

const char* const MODE_TO_EXEC[] = { EXEC_IDLE, EXEC_GLOBAL, EXEC_EDIT };

#endif
