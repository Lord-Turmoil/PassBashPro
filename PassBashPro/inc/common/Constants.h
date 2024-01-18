/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Constants.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For global constants.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// Size.
constexpr int PASH_BUFFER_SIZE = 1024;

constexpr int LOGGER_BUFFER_SIZE = PASH_BUFFER_SIZE;
constexpr int EXEC_BUFFER_SIZE = PASH_BUFFER_SIZE;

constexpr int PASSWORD_BUFFER_SIZE = 32;
constexpr int PASSWORD_MIN_LENGTH = 6;
constexpr int PASSWORD_MAX_LENGTH = 16;

constexpr int USERNAME_BUFFER_SIZE = 32;
constexpr int USERNAME_MIN_LENGTH = 3;
constexpr int USERNAME_MAX_LENGTH = 24;

constexpr int EXEC_ARG_SIZE = 32;


// XML tag identifier.
constexpr char GROUP_TAG[] = "Group";
constexpr char ITEM_TAG[] = "Item";
constexpr char ENTRY_TAG[] = "Entry";


// Special directory identifier.
constexpr char PARENT_DIR_NAME[] = "..";
constexpr char SELF_DIR_NAME[] = ".";
constexpr char ROOT_DIR_NAME[] = "./";


// Working mode.
constexpr int MODE_IDLE = 0; // not logged in
constexpr int MODE_PASH = 1;
constexpr int MODE_EDIT = 2;

// Local path.
constexpr char PASH_DIR[] = R"(.pash\)";
constexpr char HELP_ROOT_DIR[] = R"(manual\)";

constexpr const char* const HELP_DIR[] = { R"(idle\)", R"(pash\)", R"(edit\)" };

constexpr char CACHE_FILE[] = "cache";
constexpr char CONFIG_FILE[] = "config";
constexpr char DATA_FILE[] = "data";
constexpr char PROFILE_FILE[] = ".profile";


constexpr int TERMINATION = 66;

#endif
