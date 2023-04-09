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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _EXEC_H_
#define _EXEC_H_

#include "Functions.h"
#include "Service.h"


typedef int (*Exec)(int, char* []);


// Identifiers for different executable factories.
const char EXEC_GLOBAL[]  = "global";
const char EXEC_EDITOR[]  = "editor";
const char EXEC_HIDDEN[]  = "hidden";
const char EXEC_SERVICE[] = "service";


#endif
