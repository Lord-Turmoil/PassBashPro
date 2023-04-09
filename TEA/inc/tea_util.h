/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : tea_util.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Utilities for the use of TEA algorithm.                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _TEA_UTIL_H_
#define _TEA_UTIL_H_

#include "tea_core.h"

#include <cstdio>

_TEA_BEGIN

/********************************************************************
** Reader and Writer hides file details. read and write are not
** recommended to call by your self. You'd better remember to close
** them, but it's OK to leave it alone.
*/
class TEAReader
{
public:
	virtual bool Read(char* buffer, size_t nBytes) = 0;
	virtual void Close() = 0;
};

class TEAWriter
{
public:
	virtual bool Write(char* buffer, size_t nBytes) = 0;
	virtual void Close() = 0;
};

struct TEAReadBuffer
{
	const char* base;
	const char* pc;

	TEAReadBuffer(const char* _base)
	{
		base = _base;
		pc = base;
	}
};

struct TEAWriteBuffer
{
	char* base;
	char* pc;

	TEAWriteBuffer(char* _base)
	{
		base = _base;
		pc = base;
	}
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Readers
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class TEAFileReader : public TEAReader
{
public:
	TEAFileReader(FILE* input) : m_input(input) {}
	~TEAFileReader() { Close(); }

	virtual bool Read(char* buffer, size_t nBytes);
	virtual void Close();

private:
	FILE* m_input;
};

// No overflow check! Must terminate with '\0'
class TEABufferReader : public TEAReader
{
public:
	TEABufferReader(const char* buffer) : m_buffer(buffer) {}
	~TEABufferReader() { Close(); }

	virtual bool Read(char* buffer, size_t nBytes);
	virtual void Close();

private:
	TEAReadBuffer m_buffer;
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Writers
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class TEAFileWriter : public TEAWriter
{
public:
	TEAFileWriter(FILE* output) : m_output(output) {}
	~TEAFileWriter() { Close(); }

	virtual bool Write(char* buffer, size_t nBytes);
	virtual void Close();

private:
	FILE* m_output;
};

// No overflow check!
class TEABufferWriter : public TEAWriter
{
public:
	TEABufferWriter(char* buffer) : m_buffer(buffer) {}
	~TEABufferWriter() { Close(); }

	virtual bool Write(char* buffer, size_t nBytes);
	virtual void Close();

private:
	TEAWriteBuffer m_buffer;
};

_TEA_END

#endif
