/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : ExecUtil.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Utility functions for executables.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _EXEC_UTIL_
#define _EXEC_UTIL_

#include "../common/Colors.h"
#include "xml.h"

#include <string>

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Common utilities.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int _PrintPrompt();

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Advanced argument parsing.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int _ParseOptionalArgs(int argc, char* argv[], std::string& _1);
int _ParseArgs(int argc, char* argv[], std::string& _1);
int _ParseArgs(int argc, char* argv[], std::string& _1, std::string& _2);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Common operations.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int _ShowItem(XMLElementPtr node, bool detail, const char* key = nullptr, WORD color = 0);
int _ShowItemSimple(XMLElementPtr node, bool detail, const char* key = nullptr, WORD color = 0);

// check if is password entry.
bool _IsSensitive(const char* descr);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Password operations.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// fill password with '/0'.
void _FormatPassword(char* password);
void _FormatPassword(const char* buffer, char* password);

void _HashPassword(char* hashPass);
void _HashPassword(const char* password, char* hashPass);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Logs.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
#define EXEC_PRINT_MSG(...) cnsl::InsertText(MESSAGE_COLOR, __VA_ARGS__)
#define EXEC_PRINT_ERR(...) cnsl::InsertText(ERROR_COLOR, __VA_ARGS__)

#endif
