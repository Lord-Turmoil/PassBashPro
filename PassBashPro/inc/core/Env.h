/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Env.h                                     *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For user environment info.                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _ENV_H_
#define _ENV_H_

#include "../common/Constants.h"
#include "PassDoc.h"

#include <string>


/********************************************************************
** All info needed for a user.
*/
struct Env
{
	std::string username;

	std::string configPath;
	std::string dataPath;

	char password[PASSWORD_BUFFER_SIZE];
};

#endif