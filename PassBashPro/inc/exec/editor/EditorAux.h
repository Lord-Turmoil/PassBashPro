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

extern const char EDIT_IGNORE[];
extern const int EDIT_KEY_MAX_LENGTH;
extern const int EDIT_VALUE_MAX_LENGTH;
extern const int EDIT_WEIGHT_MAX_LENGTH;

extern XMLElementPtr _edit_item;
extern std::string _edit_item_path;
extern cnsl::InputHistory _edit_history;

void _edit_print_header(bool showPrompt = false);
void _edit_print_footer();

void _edit_print_prompt();

int _edit_parse_cmd(char* cmd, char** type, char** arg);
int _edit_parse_params(char* arg, int argc, const char* params[]);

int _set_key(const char* idStr, const char* newKey);
int _set_value(const char* idStr, const char* value);
int _set_weight(const char* idStr, const char* weightStr);
int _set_entry(const char* key, const char* value, const char* weightStr);

#endif
