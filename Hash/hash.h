/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Hash                                      *
 *                                                                            *
 *                      File Name : hash.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 18, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Header file for hash algorithms.                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _HASH_H_
#define _HASH_H_

#include "inc/md5_core.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "hash_b.lib")
#else
#pragma comment(lib, "hash.lib")
#endif

#endif
