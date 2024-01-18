/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : TEA                                       *
 *                                                                            *
 *                      File Name : tea_defines.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Basic defines.                                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#ifndef _TEA_DEFINES_H_
#define _TEA_DEFINES_H_

#define _TEA_BEGIN	namespace tea {
#define _TEA_END	}
#define _TEA		::tea::

_TEA_BEGIN
using DATA = unsigned long;

/********************************************************************
** If TEA_CRACK is defined, the encrypted file will include original
** file content for debug purpose, or as weak encryption.
*/
#ifdef TEA_CRACK
#define TEA_K 2
#else
#define TEA_K 1
#endif

constexpr size_t NCHAR = 2 * sizeof(long); //  64 bit =  8 Byte =  8 char
constexpr size_t KCHAR = TEA_K * NCHAR; // 128 bit = 16 Byte = 16 char

_TEA_END

#endif
