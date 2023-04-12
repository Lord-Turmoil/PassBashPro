/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Main.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Main entrance of PassBash.                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../inc/common/Macros.h"
#include "../inc/common/Error.h"
#include "../inc/common/Logger.h"
#include "../inc/exec/ExecHost.h"
#include "../inc/exec/Init.h"

int main(int argc, char* argv[])
{
	InitExecHost();
	InitConsole();

	PASH_PANIC_ON(
		ExecHost::GetInstance()->execl(EXEC_SERVICE, "start", nullptr) != 0);

#ifdef PASH_DEBUG
	LOG_PRINT_ERROR();
	LOG_PRINT_MESSAGE();
#endif

	return 0;
}
