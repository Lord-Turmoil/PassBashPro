/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : Console                                   *
 *                                                                            *
 *                      File Name : Input.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 1, 2023                           *
 *                                                                            *
 *                    Last Update : March 14, 2023                            *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For input control.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../inc/Input.h"
#include "../inc/Common.h"
#include "../inc/InputAux.h"

#pragma warning(disable: 4996)

#include <ctime>
#include <cctype>
#include <cstring>
#include <vector>
#include <string>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

_CNSL_BEGIN

char _default_buffer[INPUT_BUFFER_SIZE];

static void ResetInput(SHORT beginX, int maxLen, int& pos, int& length, char* input, char* history)
{
	if (!history)
		return;

	cnsl::Clear(beginX, beginX + length);
	cnsl::InsertText("%.*s", maxLen, history);
	length = std::min(maxLen, (int)strlen(history));
	pos = length;
	
	strncpy(input, history, length);
	input[length] = '\0';
}

typedef void _input_action_t(const InputOptions& opt, InputContext& ctx);

_input_action_t _reset_input;

static _input_action_t _insert_char;
static _input_action_t _insert_backspace;
static _input_action_t _insert_completion;

static void _special_input_handler(char ch, const InputOptions& opt, InputContext& ctx);

static _input_action_t _special_arrow_left;
static _input_action_t _special_arrow_right;
static _input_action_t _special_arrow_up;
static _input_action_t _special_arrow_down;
static _input_action_t _special_arrow_ctrl_left;
static _input_action_t _special_arrow_ctrl_right;

static _input_action_t _special_delete;
static _input_action_t _special_home;
static _input_action_t _special_end;

int GetString(char* buffer)
{
	return GetString(buffer, InputOptions());
}

int GetString(char* buffer, const InputOptions& options)
{
	InputOptions opt(options);
	opt.minLen = std::max(opt.minLen, 0);
	opt.maxLen = std::min(opt.maxLen, INPUT_BUFFER_SIZE - 1);
	
	InputContext ctx;
	ctx.buffer = buffer;
	ctx.buffer[0] = '\0';
	if (opt.history)
	{
		auto raw = opt.history->_record.data();
		ctx.history_end = raw;
		ctx.history_begin = raw + (options.history->_record.size() - 1);
		ctx.history = ctx.history_begin;
	}
	ctx.origin = GetCursorPosition();

	FlushInput();
	
	char ch;
	for (; ;)
	{
		ch = _getch();
		if (IsTerminator(ch))
		{
			if (ctx.length >= opt.minLen)
				break;	// goto normal exit (keep history)
			else if (opt.interruptible)
				return ctx.length;	// abandon history
		}
		else if ((ch == ESCAPE) && opt.interruptible)
		{
			return -1;
		}
		else if (ch == SPECIAL_LEADING)
		{
			ch = _getch();
			_special_input_handler(ch, opt, ctx);
		}
		else if (ch == BACKSPACE)
		{
			_insert_backspace(opt, ctx);
		}
		else if (ch == TAB)
		{
			_insert_completion(opt, ctx);
		}
		else if (ctx.length < opt.maxLen)
		{
			ctx.ch = ch;
			_insert_char(opt, ctx);
		}
	}

	if (ctx.history && !IsNullOrEmpty(ctx.buffer))
		opt.history->Push(ctx.buffer);

	return ctx.length;
}

static void _reset_input(const InputOptions& opt, InputContext& ctx)
{
	if (!ctx.history)
		return;

	Clear(ctx.origin.X, ctx.origin.X + ctx.length);
	InsertText("%.*s", opt.maxLen, *ctx.history);
	ctx.length = std::min(opt.maxLen, (int)strlen(*ctx.history));
	ctx.pos = ctx.length;

	strncpy(ctx.buffer, *ctx.history, ctx.length);
	ctx.buffer[ctx.length] = '\0';
}

static void _insert_char(const InputOptions& opt, InputContext& ctx)
{
	if ((ctx.length < opt.maxLen) && isprint(ctx.ch))
	{
		for (int i = ctx.length; i > ctx.pos; i--)
			ctx.buffer[i] = ctx.buffer[i - 1];
		ctx.buffer[ctx.pos] = ctx.ch;
		ctx.length++;
		ctx.buffer[ctx.length] = '\0';
		for (int i = ctx.pos; i < ctx.length; i++)
			InsertChar(opt.decoy ? opt.decoy : ctx.buffer[i]);
		ctx.pos++;
		InsertBackspace(ctx.length - ctx.pos);
	}
}

static void _insert_backspace(const InputOptions& opt, InputContext& ctx)
{
	if (ctx.pos > 0)
	{
		InsertDelete();
		for (int i = ctx.pos; i < ctx.length; i++)
		{
			InsertChar(opt.decoy ? opt.decoy : ctx.buffer[i]);
			ctx.buffer[i - 1] = ctx.buffer[i];
		}
		InsertChar(SPACE);
		InsertBackspace(ctx.length - ctx.pos + 1);
		ctx.pos--;
		ctx.length--;
		ctx.buffer[ctx.length] = '\0';
	}
}

static void _insert_completion(const InputOptions& opt, InputContext& ctx)
{
	if (!opt.completer)
		return;

	int revert;
	const char* completion = opt.completer(ctx.buffer, &revert);
	if (!completion)
		return;

	_special_end(opt, ctx);	// set cursor to the end of input
	while (revert-- > 0)
		_insert_backspace(opt, ctx);

	while (*completion)
	{
		ctx.ch = *completion;
		_insert_char(opt, ctx);
		completion++;
	}
}

static void _special_input_handler(char ch, const InputOptions& opt, InputContext& ctx)
{
	switch (ch)
	{
	case SPECIAL_ARROW_LEFT:
		_special_arrow_left(opt, ctx);
		break;
	case SPECIAL_ARROW_RIGHT:
		_special_arrow_right(opt, ctx);
		break;
	case SPECIAL_ARROW_UP:
		_special_arrow_up(opt, ctx);
		break;
	case SPECIAL_ARROW_DOWN:
		_special_arrow_down(opt, ctx);
		break;
	case SPECIAL_ARROW_CTRL_LEFT:
		_special_arrow_ctrl_left(opt, ctx);
		break;
	case SPECIAL_ARROW_CTRL_RIGHT:
		_special_arrow_ctrl_right(opt, ctx);
		break;
	case SPECIAL_DELETE:
		_special_delete(opt, ctx);
		break;
	case SPECIAL_HOME:
		_special_home(opt, ctx);
		break;
	case SPECIAL_END:
		_special_end(opt, ctx);
		break;
	default:
		break;
	}
}

static void _special_arrow_left(const InputOptions& opt, InputContext& ctx)
{
	if (ctx.pos > 0)
	{
		InsertBackspace();
		ctx.pos--;
	}
}

static void _special_arrow_right(const InputOptions& opt, InputContext& ctx)
{
	if (ctx.pos < ctx.length)
	{
		InsertChar(opt.decoy ? opt.decoy : ctx.buffer[ctx.pos]);
		ctx.pos++;
	}
}

static void _special_arrow_up(const InputOptions& opt, InputContext& ctx)
{
	if (ctx.history)
	{
		if (ctx.history > ctx.history_end)
			ctx.history--;
		_reset_input(opt, ctx);
	}
}

static void _special_arrow_down(const InputOptions& opt, InputContext& ctx)
{
	if (ctx.history)
	{
		if (ctx.history < ctx.history_begin)
			ctx.history++;
		_reset_input(opt, ctx);
	}
}

static void _special_arrow_ctrl_left(const InputOptions& opt, InputContext& ctx)
{
	while ((ctx.pos > 0) && isalnum(ctx.buffer[ctx.pos]))
	{
		InsertBackspace();
		ctx.pos--;
	}
	while ((ctx.pos > 0) && !isalnum(ctx.buffer[ctx.pos]))
	{
		InsertBackspace();
		ctx.pos--;
	}
	while ((ctx.pos > 0) && isalnum(ctx.buffer[ctx.pos]))
	{
		InsertBackspace();
		ctx.pos--;
	}
	if (!isalnum(ctx.buffer[ctx.pos]) && ctx.pos < ctx.length)
	{
		InsertChar(opt.decoy ? opt.decoy : ctx.buffer[ctx.pos]);
		ctx.pos++;
	}
}

static void _special_arrow_ctrl_right(const InputOptions& opt, InputContext& ctx)
{
	while ((ctx.pos < ctx.length) && isalnum(ctx.buffer[ctx.pos]))
	{
		InsertChar(opt.decoy ? opt.decoy : ctx.buffer[ctx.pos]);
		ctx.pos++;
	}
	while ((ctx.pos < ctx.length) && !isalnum(ctx.buffer[ctx.pos]))
	{
		InsertChar(opt.decoy ? opt.decoy : ctx.buffer[ctx.pos]);
		ctx.pos++;
	}
}

static void _special_delete(const InputOptions& opt, InputContext& ctx)
{
	if (ctx.pos < ctx.length)
	{
		for (int i = ctx.pos + 1; i < ctx.length; i++)
		{
			ctx.buffer[i - 1] = ctx.buffer[i];
			InsertChar(opt.decoy ? opt.decoy : ctx.buffer[i]);
		}
		InsertChar(SPACE);
		InsertBackspace(ctx.length - ctx.pos);
		ctx.length--;
		ctx.buffer[ctx.length] = '\0';
	}
}

static void _special_home(const InputOptions& opt, InputContext& ctx)
{
	InsertBackspace(ctx.pos);
	ctx.pos = 0;
}

static void _special_end(const InputOptions& opt, InputContext& ctx)
{
	for (int i = ctx.pos; i < ctx.length; i++)
		InsertChar(opt.decoy ? opt.decoy : ctx.buffer[i]);
	ctx.pos = ctx.length;
}

_CNSL_END