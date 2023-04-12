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

void _edit_print_header(bool showPrompt)
{
	static char buffer[128];

	sprintf_s(buffer, "Pash Editor %s", PASH_EDITOR_VERSION);
	cnsl::InsertHeaderLine(buffer, '-');

	if (showPrompt)
		cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");
}

void _edit_print_footer()
{
	cnsl::InsertNewLine();
	cnsl::InsertHeaderLine("Pash Editor End", '-');
}

void _edit_print_prompt()
{
	cnsl::InsertText("[Edit Mode]");
	cnsl::InsertText(PWD_COLOR, "%s@%s", g_env->username.c_str(), _edit_item_path.c_str());
	cnsl::InsertText(PROMPT_COLOR, "> ");
}

int _edit_parse_cmd(char* cmd, char** type, char** arg)
{
	char* p = cmd;
	while (*p && isspace(*p))
		p++;
	*type = p;
	while (*p && !isspace(*p))
		p++;

	while (*p && isspace(*p))
		*(p++) = '\0';
	*arg = p;

	p = cmd + strlen(cmd) - 1;
	while (p > *arg && isspace(*p))
		p--;
	*(p + 1) = '\0';

	return 0;
}

int _edit_parse_params(char* arg, int argc, const char* params[])
{
	if (arg == nullptr)
		return 0;

	char* context = nullptr;
	char* token = strtok_s(arg, EDIT_IGNORE, &context);
	int cnt = 0;
	while (token)
	{
		cnt++;
		if (cnt <= argc)
			params[cnt - 1] = token;
		token = strtok_s(nullptr, EDIT_IGNORE, &context);
	}

	return cnt;
}

int _set_key(const char* idStr, const char* newKey)
{
	int id;
	if (!tinyxml2::XMLUtil::ToInt(idStr, &id))
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid id format!\n");
		return 1;
	}

	XMLElementPtr entry = PashDocUtil::GetEntryNode(_edit_item, id);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with ID \"%d\" doesn't exist!\n", id);
		return 2;
	}

	if (strlen(newKey) > EDIT_KEY_MAX_LENGTH)
	{
		cnsl::InsertText(ERROR_COLOR,
						 "New key is too long! No longer than %d characters!\n",
						 EDIT_KEY_MAX_LENGTH);
		return 3;
	}
	entry->SetAttribute("key", newKey);

	_ShowItem(_edit_item, newKey, ENTRY_MODIFY_COLOR);

	return 0;
}

int _set_value(const char* idStr, const char* value)
{
	int id;
	if (!tinyxml2::XMLUtil::ToInt(idStr, &id))
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid id format!\n");
		return 1;
	}

	XMLElementPtr entry = PashDocUtil::GetEntryNode(_edit_item, id);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with ID \"%d\" doesn't exist!\n", id);
		return 2;
	}

	if (strlen(value) > EDIT_VALUE_MAX_LENGTH)
	{
		cnsl::InsertText(ERROR_COLOR,
						 "Value is too long! No longer than %d characters!\n",
						 EDIT_VALUE_MAX_LENGTH);
		return 3;
	}
	entry->SetAttribute("value", value);

	_ShowItem(_edit_item, entry->Attribute("key"), ENTRY_MODIFY_COLOR);

	return 0;
}

int _set_weight(const char* idStr, const char* weightStr)
{
	int id;
	if (!tinyxml2::XMLUtil::ToInt(idStr, &id))
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid id format!\n");
		return 1;
	}

	if (strlen(weightStr) > EDIT_WEIGHT_MAX_LENGTH)
	{
		cnsl::InsertText(ERROR_COLOR,
						 "Weight is too long! No longer than %d characters!\n",
						 EDIT_VALUE_MAX_LENGTH);
		return 2;
	}
	int weight;
	if (!tinyxml2::XMLUtil::ToInt(weightStr, &weight))
	{
		cnsl::InsertText(ERROR_COLOR, "Invalid weight format!\n");
		return 3;
	}

	XMLElementPtr entry = PashDocUtil::GetEntryNode(_edit_item, id);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with ID \"%d\" doesn't exist!\n", id);
		return 4;
	}
	entry->SetAttribute("weight", weight);

	_ShowItem(_edit_item, entry->Attribute("key"), ENTRY_MODIFY_COLOR);

	return 0;
}

int _set_entry(const char* key, const char* value, const char* weightStr)
{
	if (!key)
	{
		cnsl::InsertText(ERROR_COLOR, "Missing entry key!\n");
		return 1;
	}

	XMLElementPtr entry;
	if (key && value && weightStr)	// can create
		entry = PashDocUtil::GetOrCreateEntryNode(_edit_item, key);
	else
		entry = PashDocUtil::GetEntryNode(_edit_item, key);
	if (!entry)
	{
		cnsl::InsertText(ERROR_COLOR, "Entry with key \"%s\" doesn't exist!\n", key);
		return 2;
	}

	if (key && (strlen(key) > EDIT_KEY_MAX_LENGTH))
	{
		cnsl::InsertText(ERROR_COLOR,
						 "Key is too long! No longer than %d characters!\n",
						 EDIT_KEY_MAX_LENGTH);
		return 3;
	}
	if (value)
	{
		if (strlen(value) > EDIT_VALUE_MAX_LENGTH)
		{
			cnsl::InsertText(ERROR_COLOR,
							 "Value is too long! No longer than %d characters!\n",
							 EDIT_VALUE_MAX_LENGTH);
			return 4;
		}
		entry->SetAttribute("value", value);
	}
	if (weightStr)
	{
		if (strlen(weightStr) > EDIT_WEIGHT_MAX_LENGTH)
		{
			cnsl::InsertText(ERROR_COLOR,
							 "Key is too long! No longer than %d characters!\n",
							 EDIT_WEIGHT_MAX_LENGTH);
			return 5;
		}
		int weight = -1;
		int w;
		if (tinyxml2::XMLUtil::ToInt(weightStr, &w))
			weight = w;
		if (weight == -1)	// auto
		{
			EntryList list;
			PashDocUtil::GetEntries(_edit_item, list);
			if (list.empty())
				weight = 4;
			else
				weight = list.back().weight + 4;
		}
		entry->SetAttribute("weight", weight);
	}

	_ShowItem(_edit_item, key, ENTRY_MODIFY_COLOR);

	return 0;
}