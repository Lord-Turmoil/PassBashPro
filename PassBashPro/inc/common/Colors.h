/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Colors.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For color defines.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _COLORS_H_
#define _COLORS_H_

#include <Windows.h>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

// Colors
#define PROMPT_COLOR		FOREGROUND_LIGHT(FOREGROUND_BLUE)
#define GREETING_COLOR		FOREGROUND_GREEN
#define ERROR_COLOR			FOREGROUND_RED
#define MESSAGE_COLOR		FOREGROUND_YELLOW
#define GROUP_COLOR			FOREGROUND_CYAN
#define PWD_COLOR			FOREGROUND_MAGENTA
#define ITEM_COLOR			FOREGROUND_LIGHT(FOREGROUND_MAGENTA)
#define ENTRY_MODIFY_COLOR	FOREGROUND_LIGHT(FOREGROUND_GREEN)
#define ENTRY_DELETE_COLOR	FOREGROUND_LIGHT(FOREGROUND_RED)

#endif