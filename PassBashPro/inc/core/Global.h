/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Global.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For global variables.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <string>


// Internal default password.
extern char g_defaultPassword[];


// Present working directory.
extern std::string g_pwd;


// Current user info.
struct Env;
extern Env g_user;


// PassDoc
struct PassDoc;
extern PassDoc g_doc;


// Default data content.
extern const char g_DEFAULT_DATA[];


// Position variables.
extern const int VAR_SIZE;
extern std::vector<std::string> g_var;


// Versions.
extern const char PASH_HOST_VERSION[];
extern const char PASH_EDITOR_VERSION[];

#endif