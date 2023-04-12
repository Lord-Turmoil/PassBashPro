/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_login.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   First login on start up.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"
#include "../../../inc/core/Profile.h"

#include <tea.h>


static char _encoded_password[PASSWORD_BUFFER_SIZE];
static char _decoded_password[PASSWORD_BUFFER_SIZE];

static void _login_header();
static void _login_footer();

static int _login_list_users();
static int _login_receive_username();
static int _login_receive_password();

static bool _login_check_env(EnvPtr env);
static bool _login_init_env(EnvPtr env);
static bool _login_init_validate();
static bool _login_validate(const char* password);

int srv_login(int argc, char* argv[])
{
	_login_header();

	PASH_TRY(_login_list_users());
	
	if (_login_receive_username() != 0)
		return -TERMINATION;
	if (!_login_check_env(g_env))
	{
		/*
		 * Uninitialized env is illegal, because password is
		 * not assigned.
		 */

		return 1;
	}
	_login_init_validate();
	if (_login_receive_password() != 0)
		return -TERMINATION;
	if (!_login_init_env(g_env))
	{
		/*
		 * Failed to load PashDoc, most probably data crash.
		 */
		return 2;
	}

	_login_footer();

	return 0;
}

static void _login_header()
{
	cnsl::InsertText(GREETING_COLOR, "Logging in to PassBash...\n\n");
}

static void _login_footer()
{
	cnsl::InsertText(GREETING_COLOR, "\nCredential confirmed!\n\n");
}

static int _login_list_users()
{
	ProfilePool* pool = ProfilePool::GetInstance();
	Profile* profile;

	cnsl::InsertText(MESSAGE_COLOR, "Available users:\n");
	for (int i = 0; i < pool->Size(); i++)
	{
		profile = &(*pool)[i];
		cnsl::InsertText("\t");
		if (profile->username == g_env->username)
			cnsl::InsertText(HIGHLIGHT_COLOR, profile->username.c_str());
		else
			cnsl::InsertText(MESSAGE_COLOR, profile->username.c_str());
		cnsl::InsertNewLine();
	}
	cnsl::InsertSplitLine('_');

	return 0;
}

/********************************************************************
** Receive username to login, and set g_env to the selected user.
*/
static int _login_receive_username()
{
	char buffer[USERNAME_BUFFER_SIZE];
	ProfilePool* pool = ProfilePool::GetInstance();

	cnsl::InsertText("Username");
	cnsl::InsertText(PROMPT_COLOR, "$ ");

	cnsl::InputOptions options;
	options.minLen = 1;
	options.maxLen = USERNAME_MAX_LENGTH;
	options.interruptible = true;

	for (; ; )
	{
		int ret = cnsl::GetString(buffer, options);
		while (ret < options.minLen)
		{
			if (ret == -1)
				return 1;
			cnsl::Clear(0);
			cnsl::InsertText("Username");
			cnsl::InsertText(PROMPT_COLOR, "$ ");
			ret = cnsl::GetString(buffer, options);
		}
		strstrip(buffer);
		if (!pool->Get(buffer))
		{
			cnsl::InsertNewLine();
			EXEC_PRINT_ERR("Username doesn't exist!");
			Sleep(500);
			cnsl::Clear(0);
			cnsl::InsertReverseLineFeed();
			cnsl::Clear(0);
			cnsl::InsertText("Username");
			cnsl::InsertText(PROMPT_COLOR, "$ ");
		}
		else
			break;
	}
	cnsl::InsertNewLine();

	Profile* profile = pool->Get(buffer);
	g_env = CreateEnv(profile);

	return 0;
}

/********************************************************************
** Receive password and make validation. Set g_env->password to the
** correct password after validation.
*/
static int _login_receive_password()
{
	char buffer[PASSWORD_BUFFER_SIZE];
	int ret = 0;

	cnsl::InputOptions options;
	options.minLen = 0;
	options.maxLen = PASSWORD_MAX_LENGTH;
	options.decoy = '*';
	options.interruptible = true;

	cnsl::InsertText("Password");
	cnsl::InsertText(PROMPT_COLOR, "$ ");
	do
	{
		ret = cnsl::GetString(buffer, options);
		if (ret == -1)
			return 1;
	} while (ret == 0);
	_FormatPassword(buffer);

	while (!_login_validate(buffer))
	{
		cnsl::InsertNewLine();
		cnsl::InsertText(ERROR_COLOR, "WRONG PASSWORD!");
		Sleep(800);
		cnsl::Clear(0);
		cnsl::InsertReverseNewLine();
		cnsl::Clear(0);

		cnsl::InsertText("Password");
		cnsl::InsertText(PROMPT_COLOR, "$ ");
		do
		{
			ret = cnsl::GetString(buffer, options);
			if (ret == -1)
				return 1;
		} while (ret == 0);
		_FormatPassword(buffer);
	}

	_FormatPassword(buffer, g_env->password);
	
	return 0;
}

static bool _login_check_env(EnvPtr env)
{
	return (FileUtil::Exists(env->configPath.c_str()) &&
			FileUtil::Exists(env->dataPath.c_str()));
}

static bool _login_init_env(EnvPtr env)
{
	return g_doc.Load(env);
}

static bool _login_init_validate()
{
	FILE* input;
	const char* configPath = g_env->configPath.c_str();
	if (fopen_s(&input, configPath, "rb") != 0)
	{
		LOG_ERROR("Failed to open file \"%s\"", configPath);
		return false;
	}

	char buffer[32];
	tea::TEAFileReader* reader = new tea::TEAFileReader(input);
	tea::TEABufferWriter* writer = new tea::TEABufferWriter(_encoded_password);
	reader->Read(buffer, PASSWORD_MAX_LENGTH);
	writer->Write(buffer, PASSWORD_MAX_LENGTH);
	delete reader;
	delete writer;
}

static bool _login_validate(const char* password)
{
	tea::TEABufferReader* reader = new tea::TEABufferReader(_encoded_password);
	tea::TEABufferWriter* writer = new tea::TEABufferWriter(_decoded_password);
	tea::decode(reader, writer, password);
	delete reader;
	delete writer;

	return _STR_SAME(password, _decoded_password);
}
