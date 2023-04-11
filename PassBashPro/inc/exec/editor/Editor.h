/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : EditorFunctions.h                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For password editor functions.                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _EDITOR_FUNCTIONS_H_
#define _EDITOR_FUNCTIONS_H_

#include <tinyxml.h>
#include <string>
#include <cnsl.h>

// static void _edit_usage()
// static int _edit_parse_args(int argc, char* argv[], std::string& path)
// static void _edit_parse_cmd(char* cmd, char*& type, char*& arg);
// static void _edit_print_prompt();

int exec_edit_host(int argc, char* argv[]);

int exec_edit_edit(int argc, char* argv[]);
int exec_edit_help(int argc, char* argv[]);
int exec_edit_clear(int argc, char* argv[]);
int exec_edit_see(int argc, char* argv[]);
int exec_edit_set(int argc, char* argv[]);
int exec_edit_setk(int argc, char* argv[]);
int exec_edit_setv(int argc, char* argv[]);
int exec_edit_setw(int argc, char* argv[]);
int exec_edit_unset(int argc, char* argv[]);
int exec_edit_unknown(int argc, char* argv[]);
int exec_edit_exit(int argc, char* argv[]);

// static int _set_key(const char* idStr, const char* newKey);
// static int _set_value(const char* idStr, const char* value);
// static int _set_weight(const char* idStr, const char* weightStr);
// static int _set_entry(const char* key, const char* value, const char* weightStr);
// static int _edit_parse_params(char* arg, int argc, const char* params[]);
// static void _set_usage();
// static void _setk_usage();
// static void _setv_usage();
// static void _setw_usage();
// static void _unset_usage();

#endif