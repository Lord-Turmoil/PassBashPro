/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : FileUtil.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For file operations.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

/********************************************************************
** A collection of file operations.
** Since CreateFile and CreateDirectory are... occupied by Windows
** macros, so we have to use alternative names...
*/
class FileUtil
{
public:
	static bool Exists(const char* path);
	
	static bool NewFile(const char* path);
	static bool NewFile(const char* path, const char* filename);

	static bool NewDirectory(const char* path, bool hidden = false);
	static bool NewDirectory(const char* path, const char* dirname, bool hidden = false);

private:
	FileUtil() {}
};

#endif