/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : Input.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 1, 2023                           *
 *                                                                            *
 *                    Last Update : April 13, 2023                            *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For input control.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#ifndef _CNSL_INPUT_H_
#define _CNSL_INPUT_H_

#include "Console.h"
#include "Macros.h"
#include "Output.h"

#include <sstream>
#include <vector>

_CNSL_BEGIN
constexpr int INPUT_BUFFER_SIZE = 128;
extern char _default_buffer[INPUT_BUFFER_SIZE];

struct InputHistory
{
    InputHistory();
    ~InputHistory();

    void Push(const char* history);
    void Clear();

    std::vector<char*> _record;
};

using Completer = const char* (*)(const char*, int*);
using CharsetVerifier = bool (*)(char);

struct InputOptions
{
    int minLen;
    int maxLen;

    InputHistory* history;

    Completer completer;
    CharsetVerifier verifier;

    // The default value before input.
    const char* placeholder;

    char decoy;
    bool interruptible;

    InputOptions()
        : minLen(1), maxLen(INPUT_BUFFER_SIZE - 1), history(nullptr), completer(nullptr), verifier(nullptr),
          placeholder(nullptr), decoy(0), interruptible(false)
    {
    }

    InputOptions(int _minLen, int _maxLen, char _decoy = 0, bool _interruptible = false)
        : minLen(_minLen), maxLen(_maxLen), history(nullptr), completer(nullptr), verifier(nullptr),
          placeholder(nullptr), decoy(_decoy), interruptible(_interruptible)
    {
    }
};

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Input Control
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int GetString(char* buffer, const InputOptions& options);

// Get a string with default options.
int GetString(char* buffer);

template <typename _Ty> bool DefaultVerifier(_Ty value)
{
    return true;
}

template <typename _Ty> bool GetNumber(_Ty* value, bool (*verifier)(_Ty), const char* prompt)
{
    COORD origin = GetCursorPosition();

    GetString(_default_buffer);
    std::stringstream stream(_default_buffer);

    _Ty val;
    bool err = false;
    if (stream >> val)
    {
        if (verifier && !verifier(val))
            err = true;
    }
    else
        err = true;

    if (err)
    {
        Clear(origin.X);
        SetCursorPosition(origin);
        if (prompt)
        {
            InsertText(prompt);
            Sleep(1000);
            Clear(origin.X);
            SetCursorPosition(origin);
        }
    }
    else
        *value = val;

    return !err;
}

template <typename _Ty> bool GetNumber(_Ty* value)
{
    return GetNumber(value, DefaultVerifier<_Ty>, nullptr);
}

template <typename _Ty> bool GetNumber(_Ty* value, bool (*verifier)(_Ty))
{
    return GetNumber(value, verifier, nullptr);
}

template <typename _Ty> bool GetNumber(_Ty* value, const char* prompt) // error prompt
{
    return GetNumber(value, DefaultVerifier, prompt);
}

void FlushInput();

// Wait for key to continue, 0 means any.
void WaitForKey(const char* prompt, char key = 0);

_CNSL_END

#endif
