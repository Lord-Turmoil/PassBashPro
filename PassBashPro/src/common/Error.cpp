/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Error.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 12, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   None                                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/common/Error.h"
#include "../../inc/common/Logger.h"

#include <cstdio>
#include <cstdarg>
#include <cnsl.h>

void _panic(const char* filename, int line, const char* func, const char* format, ...)
{
    static char message[1024];

    va_list va;
    va_start(va, format);
    vsprintf_s(message, format, va);
    va_end(va);

    printf("File: '%s'\n", filename);
    printf("Line: '%d'\n", line);
    printf("Func: '%s'\n", func);
    printf(message);

    printf("\n\nPash aborted.\n");

    LOG_PRINT_MESSAGE();
    LOG_PRINT_ERROR();

    exit(-1);
}
