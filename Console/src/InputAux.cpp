/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : InputAux                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 14, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For auxiliary input functions.                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../inc/Macros.h"
#include "../inc/Input.h"
#include "../inc/InputAux.h"

#include <conio.h>
#include <cstring>

_CNSL_BEGIN
static constexpr char INPUT_TERMINATOR[] = "\n\r";


bool IsTerminator(char ch)
{
    for (const char* p = INPUT_TERMINATOR; *p; p++)
    {
        if (*p == ch)
            return true;
    }

    return false;
}


bool IsNullOrEmpty(const char* str)
{
    return !str || !*str;
}


bool IsEqual(const char* str1, const char* str2)
{
    return strcmp(str1, str2) == 0;
}


/*
 * return the position after the end of the same part.
 * Return value:
 *   nullptr: str and prefix are the same
 *   str    : str does not begin with prefix
 */
const char* BeginsWith(const char* str, const char* prefix)
{
    if (IsNullOrEmpty(str) || IsNullOrEmpty(prefix))
        return str;

    const char* original_str = str;
    while (*prefix && *str)
    {
        if (*prefix != *str)
            return original_str;
        prefix++;
        str++;
    }

    return str;
}


/*
 * return the position after the end of the same part.
 * Return value:
 *   nullptr: str does not end with suffix
 *   str    : str and suffix are the same
 */
const char* EndsWidth(const char* str, const char* suffix)
{
    if (IsNullOrEmpty(str) || IsNullOrEmpty(suffix))
        return nullptr;

    const char* str_end = str + strlen(str);
    const char* suffix_end = suffix + strlen(suffix);

    while (str_end >= str && suffix_end >= suffix)
    {
        if (*str_end != *suffix_end)
            return nullptr;
        str_end--;
        suffix_end--;
    }

    return str_end;
}


void FlushInput()
{
    static HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    FlushConsoleInputBuffer(hInput);
}


void WaitForKey(const char* prompt, char key)
{
    InsertText(prompt);

    FlushInput();

    for (int i = 0; ; i++)
    {
        char ch = _getch();
        if (ch == key)
            break;
        if (key == static_cast<char>(0))
            break;
    }
}


_CNSL_END
