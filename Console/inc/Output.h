/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : Output.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 1, 2023                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Output control.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#ifndef _CNSL_OUTPUT_H_
#define _CNSL_OUTPUT_H_

#include "Macros.h"

#include <string>
#include <Windows.h>

_CNSL_BEGIN
/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Output Control
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void InsertLineFeed(); // Row++, col does not change.
void InsertLineFeed(int n); // Insert n line feeds.
void InsertCarrige(); // Row does not change, col back to zero.
void InsertNewLine(); // Row++, col back to zero.
void InsertNewLine(int n); // Insert n new lines.

void InsertReverseLineFeed();
void InsertReverseLineFeed(int n);
void InsertReverseNewLine();
void InsertReverseNewLine(int n);

void InsertBackspace();
void InsertBackspace(int n);

void InsertDelete(); // Clear the last character and went back by one.
void InsertDelete(int n);

void InsertChar(char ch);
void InsertChar(char ch, int n);
void InsertText(const char* format, ...);
void InsertText(WORD foreground, const char* format, ...);

void InsertSplitLine(char split);
void InsertHeaderLine(const char* header, char split = '-');


template<typename _Ty>
void InsertNumber(_Ty val)
{
    static_assert(std::is_arithmetic<_Ty>::value, "Must be arithmetic!");

    InsertText(std::to_string(val).c_str());
}


_CNSL_END

#endif
