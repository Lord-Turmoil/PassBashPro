/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Auxiliary.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 15, 2023                          *
 *                                                                            *
 *                    Last Update : April 9, 2023                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Auxiliary functions.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _AUXILIARY_H_
#define _AUXILIARY_H_

#include <cstring>

#define _STR_SAME(STR1, STR2) (strcmp(STR1, STR2) == 0)
#define _STR_DIFF(STR1, STR2) (strcmp(STR1, STR2) != 0)

#define _STR_NSAME(STR1, STR2) (strncmp(STR1, STR2, strlen(STR2)) == 0)
#define _STR_NDIFF(STR1, STR2) (strncmp(STR1, STR2, strlen(STR2)) != 0)

// strip extra white spaces around
char* strstrip(char* str);

bool begins_with(const char* str, const char* prefix);
bool ends_with(const char* str, const char* suffix);


// char and wchar_t
bool widen(wchar_t* dest, const char* src);
wchar_t* widen(const char* src);
bool narrow(char* dest, const wchar_t* src);
char* narrow(const wchar_t* src);


#endif