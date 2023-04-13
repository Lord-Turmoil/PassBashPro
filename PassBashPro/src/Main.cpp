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
 * -------------------------------------------------------------------------- *
 * Comments:
 *   This is an upgraded version of the original PassBash, with extra account *
 * control features.                                                          *
 *   Initialize motivation came from Fengrubei of BUAA, which turned out to   *
 * be quite a haste. 10K+ lines of code within a week is too ridiculous, even *
 * if it has a prototype.                                                     *
 * -------------------------------------------------------------------------- *
 *                                     "                                      *
 *        I'm never gonna dance again. Guilty feet have got no rhythm.        *
 *           Though it's easy to pretend. I know you're not a fool.           *
 *               Should've known better than to cheat a friend.               *
 *                 And waste the chance that I've been given.                 *
 *                      So I'm never gonna dance again.                       *
 *                        The way I dance with you~~.                         *
 *                                     "                                      *
 *                                                        - Careless Whisper  *
 * -------------------------------------------------------------------------- *
 * Acknowledgement:                                                           *
 *    For more information, please visit http://www.tonys-studio.top.         *
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
