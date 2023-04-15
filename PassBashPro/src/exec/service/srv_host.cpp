/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_host.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Quite like a shell to receive and dispatch all user commands.            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"

#include <vector>
#include <memory>

static char HOST_IGNORE[] = " ";
static char HOST_WAITING[] = R"(-\|/)";

static cnsl::InputHistory history;

static char _buffer[EXEC_BUFFER_SIZE + 4];

static char* _cmd;
static int _argc;
static char* _argv[EXEC_ARG_SIZE];

static const int MAX_CANDIDATE_NUM = 64;
static const char* _candidates[MAX_CANDIDATE_NUM + 4];

static void _host_greet();

// revert: delete how many input
static const char* _get_completion(const char* input, int* revert);
static void _get_candidates(const std::string& path);

// Parse command environment variables.
static void _host_parse_command(char* cmd);
static int _host_peek_command();

int srv_host(int argc, char* argv[])
{
	ExecHost* host = ExecHost::GetInstance();
	
	g_mode = MODE_PASH;

	_host_greet();
	while (_host_peek_command() == 0)
	{
		cnsl::InsertNewLine();
		if (!_cmd)
			continue;

		int ret = host->execv(MODE_TO_EXEC[g_mode], _cmd, _argv);
		if (ret == -1)					// unknown
			host->execl(EXEC_HIDDEN, "unk", "unk", _cmd, nullptr);
		else if (ret == TERMINATION)	// exit
			break;
		else if (ret != 0)
		{
			LOG_ERROR("\"%s\" -- Error Code: %d", _cmd, ret);
			if (ret == -TERMINATION)
			{
				EXEC_PRINT_ERR("Critical error encountered!\n");
				EXEC_PRINT_ERR("Pash cannot proceed!\n");
				cnsl::WaitForKey("\nPress any key to exit...");
				
				return 1;
			}
		}
	}

	return 0;
}

static void _host_greet()
{
	cnsl::InsertText(GREETING_COLOR, "Pash Host fully operational!\n");
	cnsl::InsertText(MESSAGE_COLOR, "Use \"help\" for more information.\n");
	cnsl::InsertSplitLine('_');

	cnsl::FlushInput();
}

static const char* _get_completion(const char* input, int* revert)
{
	// skip command
	while (*input && !isspace(*input))
		input++;
	while (*input && isspace(*input))
		input++;
	if (!*input)
		return nullptr;

	// Get last substring to complete.
	const char* last = input;
	while (*last)
	{
		while (*last && !isspace(*last))
			last++;
		while (*last && isspace(*last))
			last++;
		if (*last)
			input = last;
	}

	std::string path(input);
	std::string parentPath;
	PashDocUtil::GetParentPath(path, parentPath);
	if (parentPath == "")
		parentPath = g_pwd;
	else
	{
		auto pos = path.find_last_of('/');
		if (pos != std::string::npos)
			input += pos + 1;
	}
	_get_candidates(parentPath);

	if (!_candidates[0])
		return nullptr;

	const char* completion = nullptr;
	*revert = 0;
	const char** candidate = _candidates;
	while (*candidate)
	{
		const char* pos = cnsl::BeginsWith(*candidate, input);
		if (!*pos)	// full match
		{
			candidate++;
			completion = *candidate ? *candidate : _candidates[0];
			*revert = (int)strlen(input);
			break;
		}
		if (pos != *candidate)	// partial match
		{
			if (!completion)
				completion = pos;
			else
				return nullptr;	// ignore multiple possible match
		}
		candidate++;
	}

	return completion;
}

static void _get_candidates(const std::string& path)
{
	XMLElementPtr node = PashDocUtil::GetNodeByPath(path);
	if (!node)
	{
		*_candidates = nullptr;
		return;
	}

	XMLElementPtrList list;
	PashDocUtil::GetChildren(node, list);

	const char** candidate = _candidates;
	for (auto it : list)
		*candidate++ = PashDocUtil::GetNodeAttr(it, "name");
	*candidate = nullptr;
}

static void _host_parse_command(char* cmd)
{
	std::string temp(cmd);
	std::string pattern;
	std::string dst;

	temp += " ";
	for (int i = 0; i < VAR_SIZE; i++)
	{
		if (g_var[i] == "")
			continue;

		pattern = " $" + std::to_string(i);
		pattern += " ";
		dst = " " + g_var[i] + " ";
		for (auto pos = temp.find(pattern);
			 pos != std::string::npos;
			 pos = temp.find(pattern))
		{
			temp.replace(pos, pattern.length(), dst);
		}
	}

	while (!temp.empty() && temp.back() == ' ')
		temp.pop_back();

	strcpy_s(cmd, EXEC_BUFFER_SIZE, temp.c_str());
}

static int _host_peek_command()
{
	_cmd = nullptr;

	_PrintPrompt();

	cnsl::InputOptions options;
	options.minLen = 0;
	options.interruptible = true;
	options.history = &history;
	options.completer = _get_completion;

	int ret = cnsl::GetString(_buffer, options);
	if (ret == -1)	// abort
		return 0;

	_host_parse_command(_buffer);

	char* context = nullptr;
	char* token = strtok_s(_buffer, HOST_IGNORE, &context);
	if (!token)	// nothing.
		return 0;

	// first arg is command name
	_argc = 0;
	_cmd = strtolower(token);
	_argv[_argc++] = _cmd;

	while (token = strtok_s(nullptr, HOST_IGNORE, &context))
		_argv[_argc++] = token;
	_argv[_argc] = nullptr;

	return 0;
}