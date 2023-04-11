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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// Size.
const int PASH_BUFFER_SIZE = 1024;

const int LOGGER_BUFFER_SIZE = PASH_BUFFER_SIZE;
const int EXEC_BUFFER_SIZE = PASH_BUFFER_SIZE;

const int PASSWORD_BUFFER_SIZE = 32;
const int PASSWORD_MIN_LENGTH = 6;
const int PASSWORD_MAX_LENGTH = 16;

const int EXEC_ARG_SIZE = 32;


// XML tag identifier.
const char GROUP_TAG[] = "Group";
const char ITEM_TAG[]  = "Item";
const char ENTRY_TAG[] = "Entry";


// Special directory identifier.
const char PARENT_DIR_NAME[] = "..";
const char SELF_DIR_NAME[]   = ".";
const char ROOT_DIR_NAME[]   = "./";


// Working mode.
const int MODE_IDLE = 0;	// not logged in
const int MODE_PASH = 1;
const int MODE_EDIT = 2;

const char* MODE_TO_EXEC[] = { EXEC_IDLE, EXEC_GLOBAL, EXEC_EDIT };

// Local path.
const char PASH_DIR[]        = R"(.pash\)";
const char HELP_ROOT_DIR[]   = R"(manual\)";

const char* HELP_DIR[] = { R"(idle\)", R"(pash\)", R"(edit\)" };

const char CONFIG_FILE[]  = "config";
const char DATA_FILE[]    = "data";
const char PROFILE_FILE[] = ".profile";


const int TERMINATION = 66;

#endif