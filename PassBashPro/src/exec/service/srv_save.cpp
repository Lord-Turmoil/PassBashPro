/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_save.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 16, 2023                            *
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

#include "../../../inc/exec/service/ServiceHeader.h"

int srv_save(int argc, char* argv[])
{
	if (g_doc.Save(g_env))
	{
		EXEC_PRINT_MSG("Password successfully saved!\n");
		return 0;
	}
	else
	{
		LOG_ERROR("Failed to save password");

		EXEC_PRINT_ERR("Failed to save password!\n");
		// EXEC_PRINT_ERR("No possible solution!\n");

		return 1;
	}
}