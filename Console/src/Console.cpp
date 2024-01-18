/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : Console.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 1, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide basic console operations.                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../inc/Console.h"
#include "../inc/Output.h"

#include <cstring>
#include <cstdlib>
#include <utility>


_CNSL_BEGIN
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

static ConsoleInfo gConsoleInfo;
static HANDLE hOutput = nullptr;


ConsoleInfo::ConsoleInfo() :
    width(DFL_CNSL_WIDTH), height(DFL_CNSL_HEIGHT),
    title(nullptr), copyright(nullptr), author(nullptr),
    headerPrinter(nullptr), overflowReprint(true), headerReprint(true)
{
}


ConsoleInfo::~ConsoleInfo()
{
    if (title)
        free(title);
    if (copyright)
        free(copyright);
    if (author)
        free(author);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Cursor Control
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static constexpr char DFL_TITLE[] = "Demo Project";
static constexpr char DFL_COPYRIGHT[] = "Tony's Studio 2020 - 2023";
static constexpr char DFL_AUTHOR[] = "Tony Skywalker";


const ConsoleInfo* GetConsoleInfo()
{
    return &gConsoleInfo;
}


static bool IsValidPosition(const COORD& coord)
{
    return ((coord.X < gConsoleInfo.size.X) && (coord.Y < gConsoleInfo.size.Y));
}


void InitConsoleSize(SHORT width, SHORT height)
{
    char msg[48];
    sprintf_s(msg, "mode con cols=%hd", width);
    system(msg);
}


void InitConsole(SHORT width, SHORT height)
{
    SetConsoleSize(width, height);
    RestoreTextAttribute();
    SetCursorPosition(ORIGIN);

    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    SetHeader(DFL_TITLE, DFL_COPYRIGHT, DFL_AUTHOR);
    SetHeaderPrinter(DefaultHeaderPrinter);
}


void SetConsoleSize(SHORT width, SHORT height)
{
    gConsoleInfo.width = width;
    gConsoleInfo.height = height;
}


SHORT GetConsoleWidth()
{
    return gConsoleInfo.width;
}


SHORT GetConsoleHeight()
{
    return gConsoleInfo.height;
}


COORD GetCursorPosition()
{
    CONSOLE_SCREEN_BUFFER_INFO pBuffer;

    GetConsoleScreenBufferInfo(hOutput, &pBuffer);

    return pBuffer.dwCursorPosition;
}


COORD SetCursorPosition(const COORD& coord)
{
    CONSOLE_SCREEN_BUFFER_INFO pBuffer;

    GetConsoleScreenBufferInfo(hOutput, &pBuffer);
    COORD ret = pBuffer.dwCursorPosition;

    if (IsValidPosition(coord))
        SetConsoleCursorPosition(hOutput, coord);

    return ret;
}


bool SetCursorPosition(const COORD& coord, COORD* old)
{
    CONSOLE_SCREEN_BUFFER_INFO pBuffer;

    GetConsoleScreenBufferInfo(hOutput, &pBuffer);
    if (old)
        *old = pBuffer.dwCursorPosition;

    if (IsValidPosition(coord))
        SetConsoleCursorPosition(hOutput, coord);
    else
        return false;

    return true;
}


void HideCursor()
{
    CONSOLE_CURSOR_INFO pInfo;

    GetConsoleCursorInfo(hOutput, &pInfo);
    pInfo.bVisible = false;
    SetConsoleCursorInfo(hOutput, &pInfo);
}


void ShowCursor()
{
    CONSOLE_CURSOR_INFO pInfo;

    GetConsoleCursorInfo(hOutput, &pInfo);
    pInfo.bVisible = true;
    SetConsoleCursorInfo(hOutput, &pInfo);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Basic Control
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void Clear()
{
    system("cls");
}


void Clear(SHORT left)
{
    Clear(left, gConsoleInfo.width);
}


void Clear(SHORT left, SHORT right)
{
    right = std::min(right, gConsoleInfo.width);
    if (right <= left)
        return;

    COORD origin = GetCursorPosition();
    origin.X = left;

    if (!SetCursorPosition(origin, nullptr))
        return;

    while (left < right)
    {
        putchar(' ');
        left++;
    }

    SetCursorPosition(origin);
}


void Clear(const COORD& upperLeft, const COORD& bottomRight)
{
    SHORT XBound = std::min(bottomRight.X, gConsoleInfo.width);
    SHORT YBound = std::min(bottomRight.X, gConsoleInfo.width);

    if ((XBound <= upperLeft.X) || (YBound <= upperLeft.X))
        return;

    COORD pos = upperLeft;
    while (pos.Y < YBound)
    {
        SetCursorPosition(pos);
        Clear(pos.X, XBound);
        pos.Y++;
    }
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Text Attribute
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
static constexpr WORD DFL_FOREGROUND = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
static constexpr WORD DFL_BACKGROUND = 0;


static void ApplyTextAttribute()
{
    WORD color = gConsoleInfo.attr.foreground | gConsoleInfo.attr.background;
    SetConsoleTextAttribute(hOutput, color);
}


void SetTextAttribute(const TextAttribute& attr, TextAttribute* old)
{
    if (old)
        *old = gConsoleInfo.attr;
    gConsoleInfo.attr = attr;

    ApplyTextAttribute();
}


WORD SetTextForeground(const WORD foreground)
{
    WORD old = gConsoleInfo.attr.foreground;
    gConsoleInfo.attr.foreground = foreground;

    ApplyTextAttribute();

    return old;
}


WORD SetTextBackground(const WORD background)
{
    WORD old = gConsoleInfo.attr.background;
    gConsoleInfo.attr.background = background;

    ApplyTextAttribute();

    return old;
}


void RestoreTextAttribute()
{
    SetTextForeground(DFL_FOREGROUND);
    SetTextBackground(DFL_BACKGROUND);
    ApplyTextAttribute();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Header
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void SetHeader(
    const char* title,
    const char* copyright,
    const char* author)
{
    SetTitle(title);
    SetCopyright(copyright);
    SetAuthor(author);
}


void SetTitle(const char* title)
{
    char* old = gConsoleInfo.title;

    if (title)
    {
        gConsoleInfo.title = _strdup(title);
        if (!gConsoleInfo.title) // memory allocation failed
        {
            gConsoleInfo.title = old;
            return;
        }
    }
    else
        gConsoleInfo.title = nullptr;

    if (old)
        free(old);
}


void SetCopyright(const char* copyright)
{
    char* old = gConsoleInfo.copyright;

    if (copyright)
    {
        gConsoleInfo.copyright = _strdup(copyright);
        if (!gConsoleInfo.copyright) // memory allocation failed
        {
            gConsoleInfo.copyright = old;
            return;
        }
    }
    else
        gConsoleInfo.copyright = nullptr;

    if (old)
        free(old);
}


void SetAuthor(const char* author)
{
    char* old = gConsoleInfo.author;

    if (author)
    {
        gConsoleInfo.author = _strdup(author);
        if (!gConsoleInfo.author) // memory allocation failed
        {
            gConsoleInfo.author = old;
            return;
        }
    }
    else
        gConsoleInfo.author = nullptr;

    if (old)
        free(old);
}


void OverflowReprint(bool reprint)
{
    gConsoleInfo.overflowReprint = reprint;
}


void HeaderReprint(bool reprint)
{
    gConsoleInfo.headerReprint = reprint;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Advanced Print
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

void SetHeaderPrinter(void (*printer)(void))
{
    gConsoleInfo.headerPrinter = printer;
}


void DefaultHeaderPrinter()
{
    char buffer[128];

    TextAttribute attr;
    TextAttribute old;

    attr.foreground = FOREGROUND_WHITE;
    attr.background = BACKGROUND_BLACK;
    SetTextAttribute(attr, &old);

    if (gConsoleInfo.title)
        InsertHeaderLine(gConsoleInfo.title, '-');
    if (gConsoleInfo.author)
    {
        sprintf_s(buffer, "Programmer - %s", gConsoleInfo.author);
        InsertHeaderLine(buffer, ' ');
    }
    if (gConsoleInfo.copyright)
    {
        sprintf_s(buffer, "Copyright (C) %s", gConsoleInfo.copyright);
        InsertHeaderLine(buffer, ' ');
    }

    InsertSplitLine('_');

    InsertNewLine();

    SetTextAttribute(old, nullptr);
}


void Print()
{
    if (gConsoleInfo.headerPrinter)
        gConsoleInfo.headerPrinter();
}


void Reprint()
{
    if (gConsoleInfo.overflowReprint)
    {
        Clear();
        if (gConsoleInfo.headerReprint && gConsoleInfo.headerPrinter)
            gConsoleInfo.headerPrinter();
    }
    else
    {
        putchar('\n');
    }
}


_CNSL_END
