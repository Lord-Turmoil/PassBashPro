/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : TEA                                       *
 *                                                                            *
 *                      File Name : tea_core.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Core algorithm for TEA algorithm.                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _TEA_CORE_H_
#define _TEA_CORE_H_

#include "tea_defines.h"

_TEA_BEGIN

// core algorithm
// v[2]: 8 characters to be encrypted =  8 Byte
// w[2]: save encrypted 8 characters  =  8 Byte
// k[4]: the key                      = 16 Byte
// it only encrypt/decrypt part of the data
void encipher(const DATA* const v, DATA* const w, const DATA* const k);
void decipher(const DATA* const v, DATA* const w, const DATA* const k);

// easy way to use
class TEAReader;
class TEAWriter;
// key must not be longer than 16!
void encode(TEAReader* input, TEAWriter* output, const char* key);
void decode(TEAReader* input, TEAWriter* output, const char* key);

_TEA_END

#endif