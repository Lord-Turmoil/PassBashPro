/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Macros.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Macros for whole project.                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _MACROS_H_
#define _MACROS_H_

#if defined(_DEBUG) || defined(DEBUG)
#define PASH_DEBUG
#endif

#ifdef PASH_DEBUG
#include <assert.h>
#define PASH_ASSERT(expression) assert(expression)
#else
#define PASH_ASSERT(expression)
#endif

// Whether enable cheat test or not.
#define PASH_CHEAT 1

#ifdef PASH_DEBUG
#define PASH_HIDE_ROOT 0
#else
#define PASH_HIDE_ROOT 1
#endif

#endif
