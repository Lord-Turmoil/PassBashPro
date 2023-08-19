/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : InputAux.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 14, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Auxiliary functions for input.                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "Macros.h"
#include "Input.h"

#include <cstring>
#include <Windows.h>

_CNSL_BEGIN
    struct InputContext
    {
        char* buffer;
        char** history_end;
        char** history_begin;
        char** history;
        int pos;
        int length;
        COORD origin;

        char ch;

        InputContext() :
            buffer(nullptr),
            history_end(nullptr),
            history_begin(nullptr),
            history(nullptr),
            pos(0),
            length(0),
            origin({0, 0}),
            ch(0)
        {
        }
    };

    bool IsTerminator(char ch);

    bool IsNullOrEmpty(const char* str);
    bool IsEqual(const char* str1, const char* str2);

    const char* BeginsWith(const char* str, const char* prefix);
    const char* EndsWidth(const char* str, const char* suffix);

    void FlushInput();

    void WaitForKey(const char* prompt, char key);

_CNSL_END
