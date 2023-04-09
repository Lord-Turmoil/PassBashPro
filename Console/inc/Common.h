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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _CNSL_COMMON_H_
#define _CNSL_COMMON_H_

#include "Macros.h"

_CNSL_BEGIN

const char LINE_FEED	= '\n';
const char CHARRIGE		= '\r';
const char BACKSPACE	= '\b';
const char SPACE		= ' ';
const char ESCAPE       = 27;
const char TAB			= '\t';

const char SPECIAL_LEADING     = (char)224;
const char SPECIAL_ARROW_UP    = 72;
const char SPECIAL_ARROW_DOWN  = 80;
const char SPECIAL_ARROW_LEFT  = 75;
const char SPECIAL_ARROW_RIGHT = 77;
const char SPECIAL_DELETE      = 83;
const char SPECIAL_HOME        = 71;
const char SPECIAL_END         = 79;

const char SPECIAL_ARROW_CTRL_UP    = (char)(-115);
const char SPECIAL_ARROW_CTRL_DOWN  = (char)(-111);
const char SPECIAL_ARROW_CTRL_LEFT  = 115;
const char SPECIAL_ARROW_CTRL_RIGHT = 116;

_CNSL_END

#endif
