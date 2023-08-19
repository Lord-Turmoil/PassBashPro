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

static constexpr int MAX_CANDIDATE_NUM = 64;
static const char* _candidates[MAX_CANDIDATE_NUM + 4];

static const char* _get_completion(const char* input, int* revert);


static void _login_header();
static void _login_footer();

static int _login_list_users();
static int _login_receive_username();
static int _login_receive_password();

static bool _login_check_env(EnvPtr env);
static bool _login_init_env(EnvPtr env);

static bool _confirm_abort();

static int _handle_user_not_exists(const char* username);

int srv_login(int argc, char* argv[])
{
    // There must be at least one possible user!
    PASH_PANIC_ON(g_env == nullptr);

    _login_header();

    PASH_TRY(_login_list_users());

    int ret;

RE_LOGIN:
    ret = _login_receive_username();
    while (ret != 0)
    {
        if (ret == TERMINATION)
        {
            cnsl::InsertNewLine();
            if (_confirm_abort())
                return TERMINATION;
            cnsl::Clear(0);
            cnsl::InsertReverseLineFeed();
            cnsl::Clear(0);
        }
        else
            return ret;
        ret = _login_receive_username();
    }

    if (!_login_check_env(g_env))
    {
        /*
         * Uninitialized env is illegal, because password is
         * not assigned.
         */

        return 1;
    }
    VerifyProfileInit(g_env);

    ret = _login_receive_password();
    while (ret != 0)
    {
        if (ret == TERMINATION)
        {
            cnsl::InsertNewLine();
            if (_confirm_abort())
                return TERMINATION;
            cnsl::Clear(0);
            cnsl::InsertReverseLineFeed();
            cnsl::Clear(0);
            cnsl::InsertReverseLineFeed();
            cnsl::Clear(0);

            // Ahhh!!! A goto!!!
            goto RE_LOGIN;
        }
        return ret;
        ret = _login_receive_password();
    }

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
    ProfilePoolPtr pool = ProfilePool::GetInstance();
    ProfilePtr profile;

    const char** candidate = _candidates;

    cnsl::InsertText(MESSAGE_COLOR, "Available users:\n");
    for (int i = 0; i < pool->Size(); i++)
    {
        profile = (*pool)[i];
        cnsl::InsertText("\t");
        if (profile->username == g_env->username)
            cnsl::InsertText(HIGHLIGHT_COLOR, profile->username.c_str());
        else
            cnsl::InsertText(profile->username.c_str());
        *candidate++ = profile->username.c_str();
        cnsl::InsertNewLine();
    }
    cnsl::InsertSplitLine('-');
    *candidate = nullptr;

    return 0;
}

/********************************************************************
** Receive username to login, and set g_env to the selected user.
*/
static int _login_receive_username()
{
    char buffer[USERNAME_BUFFER_SIZE];
    ProfilePoolPtr pool = ProfilePool::GetInstance();

    cnsl::InsertText("Username");
    cnsl::InsertText(PROMPT_COLOR, "$ ");

    cnsl::InputOptions options;
    options.minLen = 1;
    options.maxLen = USERNAME_MAX_LENGTH;
    options.interruptible = true;
    options.placeholder = g_env->username.c_str();
    options.verifier = UsernameVerifier;
    options.completer = _get_completion;

    for (; ;)
    {
        int ret = GetString(buffer, options);
        while (ret < options.minLen)
        {
            if (ret == -1)
                return TERMINATION;
            cnsl::Clear(0);
            cnsl::InsertText("Username");
            cnsl::InsertText(PROMPT_COLOR, "$ ");
            ret = GetString(buffer, options);
        }
        strstrip(buffer);
        if (!pool->Get(buffer))
        {
            if (_handle_user_not_exists(buffer) != 0)
                return -1; // create new user

            cnsl::InsertText("Username");
            cnsl::InsertText(PROMPT_COLOR, "$ ");
        }
        else
            break;
    }
    cnsl::InsertNewLine();

    ProfilePtr profile = pool->Get(buffer);
    g_env = CreateEnv(profile);

    UpdateCache();

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
    options.verifier = PasswordVerifier;

    cnsl::InsertText("Password");
    cnsl::InsertText(PROMPT_COLOR, "$ ");
    do
    {
        ret = GetString(buffer, options);
        if (ret == -1)
            return TERMINATION;
    }
    while (ret == 0);
    _FormatPassword(buffer);

    while (!VerifyProfile(buffer))
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
            ret = GetString(buffer, options);
            if (ret == -1)
                return TERMINATION;
        }
        while (ret == 0);
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

static bool _confirm_abort()
{
    EXEC_PRINT_MSG("Abort login? (Y/N) ");
    cnsl::InsertText(PROMPT_COLOR, "$ ");

    char buffer[4];

    cnsl::InputOptions options;
    options.minLen = 1;
    options.maxLen = 1;
    options.verifier = YesNoVerifier;

    GetString(buffer, options);

    return tolower(buffer[0]) == 'y';
}

static const char* _get_completion(const char* input, int* revert)
{
    if (!_candidates[0])
        return nullptr;

    // Get last substring to complete.
    const char* completion = nullptr;
    *revert = 0;
    const char** candidate = _candidates;
    while (*candidate)
    {
        const char* pos = cnsl::BeginsWith(*candidate, input);
        if (!*pos) // full match
        {
            candidate++;
            completion = *candidate ? *candidate : _candidates[0];
            *revert = static_cast<int>(strlen(input));
            break;
        }
        if (pos != *candidate) // partial match
        {
            if (!completion)
                completion = pos;
            else
                return nullptr; // ignore multiple possible match
        }
        candidate++;
    }

    return completion;
}

static int _handle_user_not_exists(const char* username)
{
    cnsl::InsertNewLine();
    EXEC_PRINT_ERR("Username doesn't exist!\n");
    cnsl::InsertText("Create new user? (Y/N) ");
    cnsl::InsertText(PROMPT_COLOR, "$ ");

    char buffer[4];

    cnsl::InputOptions options;
    options.minLen = 1;
    options.maxLen = 1;
    options.verifier = YesNoVerifier;
    GetString(buffer, options);

    // restore input
    cnsl::Clear(0);
    cnsl::InsertReverseLineFeed();
    cnsl::Clear(0);
    cnsl::InsertReverseLineFeed();
    cnsl::Clear(0);

    return tolower(buffer[0]) == 'y';
}
