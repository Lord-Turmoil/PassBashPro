/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : InputHistory.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 14, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For InputHistory class.                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../inc/Macros.h"
#include "../inc/Input.h"

_CNSL_BEGIN

InputHistory::InputHistory()
{
	_record.push_back(_strdup(""));
}

InputHistory::~InputHistory()
{
	// Perhaps
	for (auto it : _record)
		free(it);
}

void InputHistory::Push(const char* history)
{
	if (!((_record.size() > 1) && (strcmp(*(_record.end() - 2), history) == 0)))
		_record.insert(_record.end() - 1, _strdup(history));
}

void InputHistory::Clear()
{
	for (auto it : _record)
		free(it);
	_record.clear();

	_record.push_back(_strdup(""));
}

_CNSL_END
