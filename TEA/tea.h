/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : TEA                                       *
 *                                                                            *
 *                      File Name : tea.h                                     *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For TEA encryption core algorithm.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#ifndef _TEA_H_
#define _TEA_H_

#include "inc/tea_core.h"
#include "inc/tea_util.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "tea_b.lib")
#else
#pragma comment(lib, "tea.lib")
#endif

#endif
