/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_remaster.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 13, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Change master password.                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"
#include "../../../inc/utility/Auxiliary.h"

static void _remaster_greeting();
static int _remaster_authorize();
static int _remaster_receive_password();
static int _remaster_reencrypt_data();
static void _remaster_abort();

int srv_remaster(int argc, char* argv[])
{
    PASH_PANIC_ON(g_env == nullptr);
    PASH_PANIC_ON(!g_doc.IsLoaded());

    _remaster_greeting();
    int ret = _remaster_authorize();
    if (ret != 0)
    {
        if (ret == TERMINATION)
        {
            _remaster_abort();
            return 0;
        }
        return ret;
    }

    ret = _remaster_receive_password();
    if (ret != 0)
    {
        if (ret == TERMINATION)
        {
            _remaster_abort();
            return 0;
        }
        return ret;
    }

    PASH_TRY(_remaster_reencrypt_data());

    return 0;
}

static void _remaster_greeting()
{
    cnsl::InsertText(MESSAGE_COLOR, "You are now changing your master password.\n");
    cnsl::InsertText(MESSAGE_COLOR, "Press ESC to abort.\n");
}

static int _remaster_authorize()
{
    char buffer[PASSWORD_BUFFER_SIZE];
    int ret;

    cnsl::InputOptions options;
    options.minLen = PASSWORD_MIN_LENGTH;
    options.maxLen = PASSWORD_MAX_LENGTH;
    options.interruptible = true;
    options.decoy = '*';
    options.verifier = PasswordVerifier;

    cnsl::InsertText(MESSAGE_COLOR, "Please enter your old master password.\n");
    cnsl::InsertText(PROMPT_COLOR, "$ ");
    do
    {
        ret = GetString(buffer, options);
        if (ret == -1)
        {
            cnsl::InsertNewLine();
            return TERMINATION;
        }
    } while (ret == 0);
    _FormatPassword(buffer);
    while (_STR_DIFF(g_env->password, buffer))
    {
        cnsl::InsertNewLine();
        cnsl::InsertText(ERROR_COLOR, "WRONG PASSWORD!");
        Sleep(800);
        cnsl::Clear(0);
        cnsl::InsertReverseNewLine();

        do
        {
            cnsl::Clear(0);
            cnsl::InsertText(PROMPT_COLOR, "$ ");
            ret = GetString(buffer, options);
            if (ret == -1)
            {
                cnsl::InsertNewLine();
                return TERMINATION;
            }
        } while (ret == 0);
        _FormatPassword(buffer);
    }

    cnsl::InsertNewLine();
    cnsl::InsertText(GREETING_COLOR, "Credential confirmed!\n");

    return 0;
}

static int _remaster_receive_password()
{
    char buffer[PASSWORD_BUFFER_SIZE];
    int ret;

    cnsl::InputOptions options;
    options.minLen = PASSWORD_MIN_LENGTH;
    options.maxLen = PASSWORD_MAX_LENGTH;
    options.interruptible = true;
    options.verifier = PasswordVerifier;

    EXEC_PRINT_MSG("Please enter new master password.\n");
    EXEC_PRINT_MSG("%d to %d characters, any ascii that is printable.\n", PASSWORD_MIN_LENGTH, PASSWORD_MAX_LENGTH);
    do
    {
        cnsl::Clear(0);
        cnsl::InsertText(PROMPT_COLOR, "$ ");
        ret = GetString(buffer, options);
        if (ret == -1)
        {
            cnsl::InsertNewLine();
            return TERMINATION;
        }
    } while (ret < 6);

    _FormatPassword(buffer, g_env->password);

    cnsl::InsertNewLine();
    cnsl::InsertText(GREETING_COLOR, "New master password saved!\n");
    EXEC_PRINT_MSG("Tips: Use clear to prevent leak of password.\n");
    // Sleep(1000);
    // cnsl::Clear();
    // cnsl::Print();

    return 0;
}

static int _remaster_reencrypt_data()
{
    PASH_PANIC_ON(SaveConfig(g_env, true));
    PASH_PANIC_ON(SaveData(g_doc, g_env, true));

    return 0;
}

static void _remaster_abort()
{
    cnsl::InsertText(MESSAGE_COLOR, "Aborted! Master password remains!\n");
}
