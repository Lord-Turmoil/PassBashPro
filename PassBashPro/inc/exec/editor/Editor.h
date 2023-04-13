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

int exec_edit_host(int argc, char* argv[]);
// int exec_edit_help(int argc, char* argv[]);
int exec_edit_clear(int argc, char* argv[]);
int exec_edit_see(int argc, char* argv[]);
int exec_edit_set(int argc, char* argv[]);
int exec_edit_setk(int argc, char* argv[]);
int exec_edit_setv(int argc, char* argv[]);
int exec_edit_setw(int argc, char* argv[]);
int exec_edit_unset(int argc, char* argv[]);
int exec_edit_unknown(int argc, char* argv[]);
int exec_edit_exit(int argc, char* argv[]);

#endif
