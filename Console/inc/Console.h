/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : Console.h                                 *
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

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "Macros.h"

#include <Windows.h>


_CNSL_BEGIN

struct TextAttribute
{
	WORD foreground;
	WORD background;
};

const int CNSL_BUFFER_SIZE = 32;

struct ConsoleInfo
{
	union
	{
		COORD size;
		struct
		{
			SHORT width;
			SHORT height;
		};
	};
	COORD pos;

	char* title;
	char* copyright;
	char* author;

	void (*headerPrinter)(void);

	bool overflowReprint;
	bool headerReprint;

	TextAttribute attr;

	ConsoleInfo();
	~ConsoleInfo();
};

// Additional colors
#define FOREGROUND_BLACK    0
#define FOREGROUND_WHITE   (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE )
#define FOREGROUND_YELLOW  (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLACK)
#define FOREGROUND_MAGENTA (FOREGROUND_RED   | FOREGROUND_BLACK | FOREGROUND_BLUE )
#define FOREGROUND_CYAN    (FOREGROUND_BLACK | FOREGROUND_GREEN | FOREGROUND_BLUE )
#define FOREGROUND_LIGHT(COLOR) ((COLOR) | FOREGROUND_INTENSITY)

#define BACKGROUND_BLACK    0
#define BACKGROUND_WHITE   (BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE )
#define BACKGROUND_YELLOW  (BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLACK)
#define BACKGROUND_MAGENTA (BACKGROUND_RED   | BACKGROUND_BLACK | BACKGROUND_BLUE )
#define BACKGROUND_CYAN    (BACKGROUND_BLACK | BACKGROUND_GREEN | BACKGROUND_BLUE )
#define BACKGROUND_LIGHT(COLOR) ((COLOR) | BACKGROUND_INTENSITY)

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Cursor Control
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
const SHORT DFL_CNSL_WIDTH = 120;
const SHORT DFL_CNSL_HEIGHT = 30;

const COORD ORIGIN = { 0, 0 };

const ConsoleInfo* GetConsoleInfo();

// for command prompt, Terminal won't be affected.
void InitConsoleSize(SHORT width, SHORT height);
void InitConsole(SHORT width = DFL_CNSL_WIDTH, SHORT height = DFL_CNSL_HEIGHT);
void SetConsoleSize(SHORT width, SHORT height);
SHORT GetConsoleWidth();
SHORT GetConsoleHeight();

COORD GetCursorPosition();
COORD SetCursorPosition(const COORD& coord);	// Return old coordinate.
bool  SetCursorPosition(const COORD& coord, COORD* old);

void HideCursor();
void ShowCursor();

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Basic Control
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void Clear();
void Clear(SHORT left);	// Clear left to end of current line.
void Clear(SHORT left, SHORT right);	// Clear left to right of current line.
void Clear(const COORD& upperLeft, const COORD& bottomRight);	// Clear an area.

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Text Attribute
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void SetTextAttribute(const TextAttribute& attr, TextAttribute* old);
WORD SetTextForeground(const WORD foreground);
WORD SetTextBackground(const WORD background);
void RestoreTextAttribute();

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Header
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void SetHeader(
	const char* title,
	const char* copyright,
	const char* author);
void SetTitle(const char* title);
void SetCopyright(const char* copyright);
void SetAuthor(const char* author);

void OverflowReprint(bool reprint);
void HeaderReprint(bool reprint);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Advanced Print
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void SetHeaderPrinter(void (*printer)(void));
void DefaultHeaderPrinter();
void Print();
void Reprint();

_CNSL_END

#endif
