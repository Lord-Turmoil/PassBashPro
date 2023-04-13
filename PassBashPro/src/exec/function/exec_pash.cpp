/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_pash.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 13, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Easter egg to print Pash logo.                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"

static const int COLOR_NUM = 12;
static const WORD COLORS[] = {
	FOREGROUND_RED,
	FOREGROUND_YELLOW,
	FOREGROUND_BLUE,
	FOREGROUND_GREEN,
	FOREGROUND_MAGENTA,
	FOREGROUND_CYAN,

	FOREGROUND_LIGHT(FOREGROUND_RED),
	FOREGROUND_LIGHT(FOREGROUND_YELLOW),
	FOREGROUND_LIGHT(FOREGROUND_BLUE),
	FOREGROUND_LIGHT(FOREGROUND_GREEN),
	FOREGROUND_LIGHT(FOREGROUND_MAGENTA),
	FOREGROUND_LIGHT(FOREGROUND_CYAN)
};

static char BANNER[] {
	"C O P Y R I G H T  (C)  T O N Y ' S  S T U D I O  2 0 2 0 - 2 0 2 3"
};

int exec_pash(int argc, char* argv[])
{
	ExecHost::GetInstance()->execl(EXEC_GLOBAL, "version", "version", nullptr);
	for (const char* p = LOGO; *p; p++)
	{
		if (*p == ':')
			cnsl::InsertText(COLORS[Random(COLOR_NUM)], "%c", *p);
		else
			cnsl::InsertText("%c", *p);
	}
	cnsl::InsertNewLine();
	cnsl::InsertChar(' ', (cnsl::GetConsoleWidth() - strlen(BANNER)) / 2);
	for (const char* p = BANNER; *p; p++)
		cnsl::InsertText(COLORS[Random(COLOR_NUM)], "%c", *p);
	cnsl::InsertNewLine();

	return 0;
}