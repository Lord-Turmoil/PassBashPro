/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_profil.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Create local profile.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"
#include "../../../inc/core/Profile.h"


std::string username;
std::string email;
std::string password;

static bool useCli;
static bool isDelete;

static void _profile_init();
static int _profile_usage();
static int _profile_parse_arg(int argc, char* argv[]);
static int _profile_cli(bool showHelp = false);
static int _profile_silent();

static int _profile_delete_confirm();
static int _profile_delete_current();
static int _profile_delete();

static int _profile_receive_username(bool showHelp = false);
static int _profile_receive_password(bool showHelp = false);

int srv_profile(int argc, char* argv[])
{
    _profile_init();

    if (argc == 0) // internal call
        return _profile_cli(true);

    if (_profile_parse_arg(argc, argv) != 0)
    {
        _profile_usage();
        return 1;
    }

    int ret;

    // -i will be ignored if -d is set
    if (isDelete)
        return _profile_delete();
    if (useCli)
        ret = _profile_cli();
    else
        ret = _profile_silent();

    if (ret == TERMINATION)
    {
        cnsl::InsertNewLine();
        return 0;
    }

    return ret;
}

static void _profile_init()
{
    username = "";
    email = "";
    password = "";

    useCli = false;
    isDelete = false;
}

static int _profile_usage()
{
    return ExecHost::GetInstance()
        ->execl(EXEC_GLOBAL, "help", "help", "profile", nullptr);
}

static int _profile_parse_arg(int argc, char* argv[])
{
    int opt;
    int arg_cnt = 0;
    int opt_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, "u:p:id"))
    {
        if (opterr != 0)
        {
            EXEC_PRINT_ERR("Argument error: %s\n", optmsg);
            err = true;
            resetopt();
            break;
        }

        switch (opt)
        {
        case 'u':
            username = optarg;
            break;
        case 'p':
            password = optarg;
            break;
        case 'i':
            useCli = true;
            break;
        case 'd':
            isDelete = true;
            break;
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
            {
                err = true;
                EXEC_PRINT_ERR(ERRMSG_TOO_MANY "\n");
            }
            break;
        case '?':
            opt_cnt++;
            err = true;
            if (opt_cnt == 1)
                EXEC_PRINT_ERR("Unknown parameter \"-%c\"\n", optopt);
            break;
        default:
            break;
        }
    }

    if (!useCli && (username.empty() || password.empty()))
    {
        EXEC_PRINT_ERR(ERRMSG_TOO_FEW "\n");
        EXEC_PRINT_MSG("Missing username or password.\n");
        err = true;
    }

    if (err)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        return 1;
    }

    return 0;
}

static int _profile_cli(bool showHelp)
{
    int ret = _profile_receive_username(showHelp);
    if (ret == 0)
    {
        ret = _profile_receive_password(showHelp);
        if (ret == 0)
            return _profile_silent();
    }

    return ret;
}

static int _profile_silent()
{
    ProfilePoolPtr pool = ProfilePool::GetInstance();

    if (pool->Get(username))
    {
        EXEC_PRINT_ERR("Username '%s' already exists!\n", username.c_str());
        return 10;
    }

    if (!VerifyUsername(username))
        return 11;
    if (!VerifyPassword(password))
        return 12;

    std::string path(PASH_DIR);
    path.append(username).append("\\");

    // use physical directory to mark profile
    if (!FileUtil::NewDirectory(path.c_str()))
        return 13;

    ProfilePtr profile(new Profile(username, path));
    pool->Add(profile);
    EnvPtr env = CreateEnv(profile);
    _FormatPassword(password.c_str(), env->password);

    PASH_PANIC_ON(InitEnvFiles(env));

    EXEC_PRINT_MSG("New profile '%s' created!\n", profile->username.c_str());

    return 0;
}

static int _profile_delete_confirm()
{
    EXEC_PRINT_MSG("Delete current profile? (Y/N) ");
    cnsl::InsertText(PROMPT_COLOR, "$ ");

    char buffer[4];

    cnsl::InputOptions options;
    options.minLen = 1;
    options.maxLen = 1;
    options.verifier = YesNoVerifier;

    GetString(buffer, options);

    return tolower(buffer[0]) != 'y';
}

static int _profile_delete_current()
{
    return DeleteProfile(ProfilePool::GetInstance()->Get(username), true);
}

static int _profile_delete()
{
    if (!VerifyUsername(username))
        return 21;
    if (!VerifyPassword(password))
        return 22;

    ProfilePtr profile = ProfilePool::GetInstance()->Get(username);
    if (!profile)
    {
        EXEC_PRINT_ERR("Profile doesn't exist!\n");
        return 23;
    }

    char _buffer[PASSWORD_BUFFER_SIZE];
    _FormatPassword(password.c_str(), _buffer);
    VerifyProfileInit(CreateEnv(profile));
    if (!VerifyProfile(_buffer))
    {
        EXEC_PRINT_ERR("Incorrect password for profile '%s'!\n", username.c_str());
        return 24;
    }

    int ret = DeleteProfile(profile);
    bool isSelf = false;
    if (ret != 0)
    {
        if (ret == 1)
        {
            if (_profile_delete_confirm() == 0)
            {
                cnsl::InsertNewLine();

                isSelf = true;
                ret = _profile_delete_current();
                if (ret != 0)
                {
                    EXEC_PRINT_ERR("Failed to delete current profile.\n");
                    return ret;
                }
                EXEC_PRINT_MSG("Current profile '%s' deleted.\n", username.c_str());
                EXEC_PRINT_MSG("Abort in 1 second.\n");
                Sleep(1000);
                return TERMINATION;
            }
            cnsl::InsertNewLine();
            EXEC_PRINT_MSG("No profile affected.\n");
            return 0;
        }
        if (ret == 2)
            EXEC_PRINT_ERR("Failed to delete profile!\n");

        return ret;
    }

    // profile->username will be modified by delete...
    EXEC_PRINT_MSG("Profile '%s' deleted.\n", username.c_str());

    return 0;
}

static int _profile_receive_username(bool showHelp)
{
    char buffer[USERNAME_BUFFER_SIZE];
    ProfilePoolPtr pool = ProfilePool::GetInstance();

    if (showHelp)
    {
        cnsl::InsertText(FOREGROUND_LIGHT(MESSAGE_COLOR),
                         "This is the identity of a profile, you can create more profiles later.\n");
    }

    cnsl::InsertText("Please enter the ");
    cnsl::InsertText(HIGHLIGHT_COLOR, "username");
    cnsl::InsertText(" for this profile:\n");

    cnsl::InsertText(MESSAGE_COLOR,
                     "%d to %d characters, only [_a-zA-Z0-9] is allowed.\n",
                     USERNAME_MIN_LENGTH, USERNAME_MAX_LENGTH);
    cnsl::InsertText(PROMPT_COLOR, "$ ");

    cnsl::InputOptions options;
    options.minLen = USERNAME_MIN_LENGTH;
    options.maxLen = USERNAME_MAX_LENGTH;
    options.interruptible = true;
    options.verifier = UsernameVerifier;

    for (; ;)
    {
        int ret = GetString(buffer, options);
        while (ret < options.minLen)
        {
            if (ret == -1)
                return TERMINATION;
            cnsl::Clear(0);
            cnsl::InsertText(PROMPT_COLOR, "$ ");
            ret = GetString(buffer, options);
        }
        strstrip(buffer);
        if (pool->Get(buffer))
        {
            cnsl::InsertNewLine();
            EXEC_PRINT_ERR("Username already exists!");
            Sleep(500);
            cnsl::Clear(0);
            cnsl::InsertReverseLineFeed();
            cnsl::Clear(0);
            cnsl::InsertText(PROMPT_COLOR, "$ ");
        }
        else
            break;
    }
    cnsl::InsertNewLine();

    username.assign(buffer);

    return 0;
}

static int _profile_receive_password(bool showHelp)
{
    char buffer[PASSWORD_BUFFER_SIZE];

    if (showHelp)
    {
        cnsl::InsertText(FOREGROUND_LIGHT(MESSAGE_COLOR),
                         "This is your master password to access all the other passwords.\n");
        cnsl::InsertText(FOREGROUND_LIGHT(MESSAGE_COLOR), "It will ");
        cnsl::InsertText(FOREGROUND_LIGHT(FOREGROUND_RED), "NOT BE STORED");
        cnsl::InsertText(FOREGROUND_LIGHT(MESSAGE_COLOR), ", so make sure to remember it.\n");
    }

    cnsl::InsertText("Please enter the ");
    cnsl::InsertText(HIGHLIGHT_COLOR, "password");
    cnsl::InsertText(" for this profile:\n");

    cnsl::InsertText(MESSAGE_COLOR,
                     "%d to %d characters, any ascii that is printable. (no space)\n",
                     PASSWORD_MIN_LENGTH, PASSWORD_MAX_LENGTH);

    cnsl::InsertText(PROMPT_COLOR, "$ ");

    cnsl::InputOptions options;
    options.minLen = PASSWORD_MIN_LENGTH;
    options.maxLen = PASSWORD_MAX_LENGTH;
    options.interruptible = true;
    options.verifier = PasswordVerifier;

    int ret = GetString(buffer, options);
    while (ret < options.minLen)
    {
        if (ret == -1)
            return TERMINATION;
        cnsl::Clear(0);
        cnsl::InsertText(PROMPT_COLOR, "$ ");
        ret = GetString(buffer, options);
    }
    cnsl::InsertNewLine();
    _FormatPassword(buffer);

    password.assign(buffer);

    return 0;
}
