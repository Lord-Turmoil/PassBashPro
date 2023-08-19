/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Error.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Handle error info.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _ERROR_H_
#define _ERROR_H_


// Try execute, and return immediately if error occurred.
#define PASH_TRY(expression)						\
	do {											\
		int ret = (expression);						\
		if (ret != 0)								\
		{											\
			LOG_ERROR("%s: %d", #expression, ret);	\
			return ret;								\
		}											\
	} while (0)


// Try execute, and abort the whole program if error occurred.
void _panic(const char*, int, const char*, const char*, ...);

#define PASH_PANIC(...) _panic(__FILE__, __LINE__, __func__, __VA_ARGS__)

#define PASH_PANIC_ON(expression)								\
	do {														\
		int ret = (expression);									\
		if (ret != 0)											\
		{														\
			PASH_PANIC("Critical Error Encountered:\n\t'"		\
						#expression "'\nError Code: %d", ret);	\
		}														\
	} while (0)


#define ERRNO_EXEC_NOT_FOUND (-1)

#endif
