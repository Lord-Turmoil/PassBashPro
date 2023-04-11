/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_edit_host.cpp                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   None                                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/editor/EditorHeader.h"

const char EDIT_IGNORE[] = "|";
const int  EDIT_KEY_MAX_LENGTH = 30;
const int  EDIT_VALUE_MAX_LENGTH = 45;
const int  EDIT_WEIGHT_MAX_LENGTH = 9;

XMLElementPtr _edit_item;
cnsl::InputHistory _edit_history;

int exec_edit_host(int argc, char* argv[])
{
	exec_edit_header(true);

	// clear previous history
	_edit_history.Clear();

	// main loop
	char buffer[PASH_BUFFER_SIZE + 4];
	int ret;
	PashDocUtil::GetNodeDirectory(item, item_path);

	cnsl::InputOptions options(0, CMD_BUFFER_SIZE, 0, true);
	options.history = &history;

	_edit_print_prompt();
	while ((ret = cnsl::GetString(buffer, options)) != -1)
	{
		cnsl::InsertNewLine();
		if (ret > 0)
		{
			char* cmd = _edit_strip_white_space(buffer);
			char* type;
			char* arg;
			_edit_parse_cmd(cmd, type, arg);

			int ret = g_editorFactory.execl(type, type, arg, nullptr);
			if (ret == -1)
				g_hiddenFactory.execl("_edit_unknown", "_edit_unknown", type, nullptr);
			else if (ret == 66)
				break;
			else if (ret != 0)
				LOG_ERROR("Editor \"%s\" -- Error Code: %d", type, ret);
		}
		_edit_print_prompt();
	}

	//end
	cnsl::InsertNewLine();
	cnsl::InsertHeaderLine("Edit End", '-');

	return 0;
}