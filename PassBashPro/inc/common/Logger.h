/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : Logger.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For error logs.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "Macros.h"
#include "Constants.h"

#include <vector>
#include <string>


enum class LoggerStatus
{
    GOOD,
    BAD
};


class Logger
{
public:
    static void LogError(const char* func, const char* format, ...);
    static void ClearErrors();
    static void PrintErrors();

    static void LogMessage(const char* func, const char* format, ...);
    static void ClearMessages();
    static void PrintMessages();

    static bool Good();

private:
    Logger()
    {
    }

    static std::vector<std::string> m_logs;
    static std::vector<std::string> m_msgs;
    static char m_buffer[LOGGER_BUFFER_SIZE];
};


#define LOG_LOC R"(In "%s": )"


#define LOG_ERROR(FORMAT, ...)   Logger::LogError(__func__, FORMAT, __VA_ARGS__)
#define LOG_MESSAGE(FORMAT, ...) Logger::LogMessage(__func__, FORMAT, __VA_ARGS__)

#define LOG_CLEAR_ERRORS()   Logger::ClearErrors()
#define LOG_CLEAR_MESSAGES() Logger::ClearMessages()

#define LOG_PRINT_ERROR()   Logger::PrintErrors()
#define LOG_PRINT_MESSAGE() Logger::PrintMessages()

#define HAS_ERROR() (!Logger::Good())
#define STATUS()    (Logger::Good())


#endif
