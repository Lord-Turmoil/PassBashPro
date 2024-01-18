/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : MD5                                       *
 *                                                                            *
 *                      File Name : md5_core.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 18, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Original File comes from Stephan Brumme.                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

// //////////////////////////////////////////////////////////
// md5.h
// Copyright (c) 2014 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#pragma once

#include "hash_defines.h"

#include <string>
#include <cstdint>

_HASH_BEGIN
class MD5 //: public Hash
{
public:
    /// split into 64 byte blocks (=> 512 bits), hash is 16 bytes long
    enum { BlockSize = 512 / 8, HashBytes = 16 };


    /// same as reset()
    MD5();

    /// compute MD5 of a memory block
    std::string operator()(const void* data, size_t numBytes);
    /// compute MD5 of a string, excluding final zero
    std::string operator()(const std::string& text);

    /// add arbitrary number of bytes
    void Add(const void* data, size_t numBytes);

    /// return latest hash as 32 hex characters
    std::string GetHash();
    /// return latest hash as bytes
    void GetHash(unsigned char buffer[HashBytes]);

    /// restart
    void Reset();

private:
    /// process 64 bytes
    void _ProcessBlock(const void* data);
    /// process everything left in the internal buffer
    void _ProcessBuffer();

    /// size of processed data in bytes
    uint64_t m_numBytes;
    /// valid bytes in m_buffer
    size_t m_bufferSize;
    /// bytes not processed yet
    uint8_t m_buffer[BlockSize];


    enum { HashValues = HashBytes / 4 };


    /// hash, stored as integers
    uint32_t m_hash[HashValues];
};


_HASH_END
