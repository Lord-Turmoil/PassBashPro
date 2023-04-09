/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : TEA                                       *
 *                                                                            *
 *                      File Name : tea_util.cpp                              *
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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../inc/tea_util.h"


_TEA_BEGIN

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Readers
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool TEAFileReader::Read(char* buffer, size_t nBytes)
{
	size_t bytes = fread(buffer, sizeof(char), nBytes, m_input);

	if (bytes == 0)
		return false;

	while (bytes < nBytes)
		buffer[bytes++] = '\0';

	return true;
}

void TEAFileReader::Close()
{
	if (m_input)
	{
		fclose(m_input);
		m_input = nullptr;
	}
}


bool TEABufferReader::Read(char* buffer, size_t nBytes)
{
	if (*m_buffer.pc == '\0')
		return false;

	size_t bytes = 0;
	while ((*m_buffer.pc != '\0') && (bytes < nBytes))
		buffer[bytes++] = *(m_buffer.pc++);
	while (bytes < nBytes)
		buffer[bytes++] = '\0';

	return true;
}

void TEABufferReader::Close()
{
	m_buffer.base = m_buffer.pc = nullptr;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Writers
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool TEAFileWriter::Write(char* buffer, size_t nBytes)
{
	size_t bytes = fwrite(buffer, sizeof(char), nBytes, m_output);

	return (bytes == nBytes);
}

void TEAFileWriter::Close()
{
	if (m_output)
	{
		fclose(m_output);
		m_output = nullptr;
	}
}


bool TEABufferWriter::Write(char* buffer, size_t nBytes)
{
	size_t bytes = 0;

	while (bytes < nBytes)
		*(m_buffer.pc++) = buffer[bytes++];
	
	// Always mark the end of the buffer.
	*m_buffer.pc = '\0';

	return true;
}

void TEABufferWriter::Close()
{
	m_buffer.base = m_buffer.pc = nullptr;
}

_TEA_END
