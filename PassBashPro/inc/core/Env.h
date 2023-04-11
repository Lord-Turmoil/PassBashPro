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
#include "PashDoc.h"

#include <string>


/********************************************************************
** All info needed for a user.
*/
struct Env
{
	// different user can have different local username
	std::string username;

	// email is used as unique identifier for online service
	std::string email;

	std::string configPath;
	std::string dataPath;
	std::string profilePath;

	char password[PASSWORD_BUFFER_SIZE];

	bool online;
};

#endif