/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Logger.cpp                                *
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
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../inc/common/Logger.h"

#include <cnsl.h>

#include <cstdarg>
#include <cstdio>

std::vector<std::string> Logger::m_logs;
std::vector<std::string> Logger::m_msgs;
char Logger::m_buffer[LOGGER_BUFFER_SIZE];

static char _format[LOGGER_BUFFER_SIZE];

static LoggerStatus status = LoggerStatus::GOOD;

void Logger::LogError(const char* func, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    if (func)
    {
        sprintf_s(_format, "In %s: %s", func, format);
        vsprintf_s(m_buffer, _format, args);
    }
    else
        vsprintf_s(m_buffer, format, args);

    va_end(args);

    m_logs.push_back(m_buffer);
    status = LoggerStatus::BAD;
}

void Logger::ClearErrors()
{
    m_logs.clear();
    status = LoggerStatus::GOOD;
}

void Logger::PrintErrors()
{
    WORD old = cnsl::SetTextForeground(FOREGROUND_RED);

    cnsl::InsertHeaderLine("Error Log", '_');
    for (auto it = m_logs.rbegin(); it != m_logs.rend(); ++it)
        cnsl::InsertText("\t%s\n", it->c_str());
    cnsl::InsertSplitLine('_');

    cnsl::SetTextForeground(old);
}

void Logger::LogMessage(const char* func, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    if (func)
    {
        sprintf_s(_format, "In %s: %s", func, format);
        vsprintf_s(m_buffer, _format, args);
    }
    else
        vsprintf_s(m_buffer, format, args);
    va_end(args);

    m_msgs.push_back(m_buffer);
}

void Logger::ClearMessages()
{
    m_msgs.clear();
}

void Logger::PrintMessages()
{
    WORD old = cnsl::SetTextForeground(FOREGROUND_LIGHT(FOREGROUND_MAGENTA));

    cnsl::InsertHeaderLine("Message Log", '_');
    for (auto it = m_msgs.rbegin(); it != m_msgs.rend(); ++it)
        cnsl::InsertText("\t%s\n", it->c_str());
    cnsl::InsertSplitLine('_');

    cnsl::SetTextForeground(old);
}

bool Logger::Good()
{
    return status == LoggerStatus::GOOD;
}
