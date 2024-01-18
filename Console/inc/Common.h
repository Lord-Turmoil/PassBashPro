/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : Common.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 1, 2023                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Some defines?                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#ifndef _CNSL_COMMON_H_
#define _CNSL_COMMON_H_

#include "Macros.h"

_CNSL_BEGIN
constexpr char LINE_FEED = '\n';
constexpr char CHARRIGE = '\r';
constexpr char BACKSPACE = '\b';
constexpr char SPACE = ' ';
constexpr char ESCAPE = 27;
constexpr char TAB = '\t';

constexpr char CTRL_BACKSPACE = 127;

constexpr char SPECIAL_LEADING = static_cast<char>(224);
constexpr char SPECIAL_ARROW_UP = 72;
constexpr char SPECIAL_ARROW_DOWN = 80;
constexpr char SPECIAL_ARROW_LEFT = 75;
constexpr char SPECIAL_ARROW_RIGHT = 77;
constexpr char SPECIAL_DELETE = 83;
constexpr char SPECIAL_CTRL_DELETE = -109;
constexpr char SPECIAL_HOME = 71;
constexpr char SPECIAL_END = 79;

constexpr char SPECIAL_ARROW_CTRL_UP = -115;
constexpr char SPECIAL_ARROW_CTRL_DOWN = -111;
constexpr char SPECIAL_ARROW_CTRL_LEFT = 115;
constexpr char SPECIAL_ARROW_CTRL_RIGHT = 116;

_CNSL_END

#endif
