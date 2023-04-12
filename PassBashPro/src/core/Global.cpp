/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Global.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 12, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   None                                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/core/Global.h"
#include "../../inc/core/PashDoc.h"
#include "../../inc/exec/Exec.h"

// Internal default password.
const char DEFAULT_PASSWORD[] = "null";

// Present working directory.
std::string g_pwd;

// Current user info.
EnvPtr g_env;
bool g_isOnline;

// PashDoc
PashDoc g_doc;

// Default data content.
const char DEFAULT_DATA[]{
R"(<?xml version="1.0" encoding="UTF-8"?>
<Group name=".">
</Group>
)"
};


// Position variables.
const int VAR_SIZE = 10;
std::vector<std::string> g_var;

// Versions.
const char PASH_HOST_VERSION[] = "3.0.0";
const char PASH_EDITOR_VERSION[] = "Pash Editor 2.0.0";

// Working mode.
int g_mode = MODE_IDLE;

// Copyright info.
const char TITLE[]     { "PassBash" };
const char COPYRIGHT[] { "Tony's Studio (C) 2020 - 2023" };
const char AUTHOR[]    { "Tony Skywalker" };