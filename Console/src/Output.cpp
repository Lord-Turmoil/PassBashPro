/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : Output.cpp                                *
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

#include "../inc/Output.h"
#include "../inc/Console.h"
#include "../inc/Common.h"

#include <cstring>
#include <cstdio>
#include <cstdarg>

_CNSL_BEGIN
    /*
    **+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    ** Output Control
    **+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    static bool IsNewLine(char ch)
    {
        return (ch == LINE_FEED) || (ch == CHARRIGE);
    }

    void InsertLineFeed()
    {
        COORD pos = GetCursorPosition();
        pos.Y++;

        if (!SetCursorPosition(pos, nullptr))
        {
            // out of bound
            Reprint();
        }
    }

    void InsertLineFeed(int n)
    {
        for (int i = 0; i < n; i++)
            InsertLineFeed();
    }

    void InsertCarrige()
    {
        putchar(CHARRIGE);
    }

    void InsertNewLine()
    {
        putchar(CHARRIGE);
        InsertLineFeed();
    }

    void InsertNewLine(int n)
    {
        for (int i = 0; i < n; i++)
            InsertNewLine();
    }

    void InsertReverseLineFeed()
    {
        COORD pos = GetCursorPosition();
        if (pos.Y > 0)
        {
            pos.Y--;
            SetCursorPosition(pos, nullptr);
        }
    }

    void InsertReverseLineFeed(int n)
    {
        for (int i = 0; i < n; i++)
            InsertReverseLineFeed();
    }

    void InsertReverseNewLine()
    {
        putchar(CHARRIGE);
        InsertReverseLineFeed();
    }

    void InsertReverseNewLine(int n)
    {
        for (int i = 0; i < n; i++)
            InsertReverseNewLine();
    }

    void InsertBackspace()
    {
        putchar(BACKSPACE);
    }

    void InsertBackspace(int n)
    {
        for (int i = 0; i < n; i++)
            InsertBackspace();
    }

    void InsertDelete()
    {
        putchar(BACKSPACE);
        putchar(SPACE);
        putchar(BACKSPACE);
    }

    void InsertDelete(int n)
    {
        for (int i = 0; i < n; i++)
            InsertDelete();
    }

    void InsertChar(const char ch)
    {
        if (ch == CHARRIGE)
            InsertCarrige();
        else if (ch == LINE_FEED)
            InsertLineFeed();
        else
            putchar(ch);
    }

    void InsertChar(const char ch, int n)
    {
        for (int i = 0; i < n; i++)
            InsertChar(ch);
    }

    void InsertText(const char* format, ...)
    {
        static char buffer[256];

        if (!format)
            return;

        va_list args;

        va_start(args, format);
        vsprintf_s(buffer, format, args);
        va_end(args);

        for (const char* p = buffer; *p; p++)
        {
            if (IsNewLine(*p))
                InsertNewLine();
            else
                InsertChar(*p);
        }
    }

    void InsertText(WORD foreground, const char* format, ...)
    {
        static char buffer[256];

        if (!format)
            return;

        va_list args;
        va_start(args, format);
        vsprintf_s(buffer, format, args);
        va_end(args);

        WORD old = SetTextForeground(foreground);
        for (const char* p = buffer; *p; p++)
        {
            if (IsNewLine(*p))
                InsertNewLine();
            else
                InsertChar(*p);
        }
        SetTextForeground(old);
    }

    void InsertSplitLine(char split)
    {
        if (IsNewLine(split))
            return;

        SHORT width = GetConsoleWidth();

        Clear(0);
        InsertCarrige();

        for (SHORT i = 0; i < width; i++)
            InsertChar(split);

        InsertNewLine();
    }

    void InsertHeaderLine(const char* header, char split)
    {
        if (!header)
            return;
        if (IsNewLine(split))
            return;

        SHORT width = GetConsoleWidth();

        Clear(0);
        InsertCarrige();

        SHORT length = static_cast<SHORT>(strlen(header));
        SHORT begin = (width - length) / static_cast<SHORT>(2);
        SHORT i;

        for (i = 0; i < begin - 1; i++)
            InsertChar(split);

        InsertChar(SPACE);
        i++;
        InsertText(header);
        i += length;
        InsertChar(SPACE);
        i++;

        for (i; i < width; i++)
            InsertChar(split);

        InsertNewLine();
    }

_CNSL_END
