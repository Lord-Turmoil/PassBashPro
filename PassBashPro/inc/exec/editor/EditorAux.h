/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : EditorAux.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
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

#pragma once

#ifndef _EDITOR_AUX_H_
#define _EDITOR_AUX_H_

#include <string>
#include <tinyxml.h>
#include <cnsl.h>

extern const char EDIT_IGNORE[] = "|";
extern const int EDIT_KEY_MAX_LENGTH = 30;
extern const int EDIT_VALUE_MAX_LENGTH = 45;
extern const int EDIT_WEIGHT_MAX_LENGTH = 9;

extern XMLElementPtr _edit_item;
extern std::string _edit_item_path;
extern cnsl::InputHistory _edit_history;

void exec_edit_header(bool showPrompt = false);
void exec_edit_footer();

#endif