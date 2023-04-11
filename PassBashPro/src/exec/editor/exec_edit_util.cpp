/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassPashPro                               *
 *                                                                            *
 *                      File Name : exec_edit_util.cpp                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Common utility functions for Editor.                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/editor/EditorHeader.h"

void exec_edit_header(bool showPrompt = false)
{
	static char buffer[128];

	sprintf(buffer, "Pash Editor %s", PASH_EDITOR_VERSION);
	cnsl::InsertHeaderLine(buffer, '-');

	if (showPrompt)
		cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");
}

void exec_edit_footer()
{
	cnsl::InsertNewLine();
	cnsl::InsertHeaderLine("Pash Editor End", '-');
}
