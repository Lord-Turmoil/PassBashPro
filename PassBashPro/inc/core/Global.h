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
#include <vector>

#include "Env.h"


// Internal default password.
extern const char DEFAULT_PASSWORD[];


// Present working directory.
extern std::string g_pwd;


// Current user info.
extern EnvPtr g_env;
extern bool g_isOnline;


// PashDoc
class PashDoc;
extern PashDoc g_doc;


// Default data content.
extern const char DEFAULT_DATA[];


// Position variables.
extern const int VAR_SIZE;
extern std::vector<std::string> g_var;


// Versions.
extern const char PASH_HOST_VERSION[];
extern const char PASH_EDITOR_VERSION[];


// Working mode.
extern int g_mode;


// Copyright info.
extern const char TITLE[];
extern const char COPYRIGHT[];
extern const char AUTHOR[];

// Logo
extern const char LOGO[];
extern const char BANNER[];
extern const char THANKS[];

#endif
