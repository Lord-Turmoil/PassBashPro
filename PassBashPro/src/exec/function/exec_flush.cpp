/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_flush.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 16, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Flush command history and variables.                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"

#include <algorithm>


int exec_flush(int argc, char* argv[])
{
	g_history.Clear();
	EXEC_PRINT_MSG("Command history flushed.\n");

	std::fill(g_var.begin(), g_var.end(), std::string());
	EXEC_PRINT_MSG("Environment variables reseted.\n");
	
	return 0;
}
