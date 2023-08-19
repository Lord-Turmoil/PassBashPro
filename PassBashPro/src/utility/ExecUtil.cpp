/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : ExecUtil.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Utility functions for executables.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../inc/exec/ExecHeader.h"
#include "../../inc/core/Env.h"

#include <regex>
#include <hash.h>


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Common utilities.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int _PrintPrompt()
{
    if (!g_env)
    {
        LOG_ERROR("No logged in user");
        return -1;
    }

    PashDocUtil::GetPresentWorkingDirectory(g_pwd);

    cnsl::InsertText(USER_COLOR, "%s", g_env->username.c_str());
    cnsl::InsertText(PROMPT_COLOR, ":");
    cnsl::InsertText(PWD_COLOR, "%s", g_pwd.c_str());
    cnsl::InsertText(PROMPT_COLOR, "\b$ ");

    return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Advanced argument parsing.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int _ParseOptionalArgs(int argc, char* argv[], std::string& _1)
{
    int opt;
    int arg_cnt = 0;
    int opt_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, ""))
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
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
                _1 = optarg;
            else if (arg_cnt == 2)
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

    if (err)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        return 1;
    }

    return 0;
}

int _ParseArgs(int argc, char* argv[], std::string& _1)
{
    int opt;
    int arg_cnt = 0;
    int opt_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, ""))
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
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
                _1 = optarg;
            else if (arg_cnt == 2)
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

    if (arg_cnt == 0)
    {
        EXEC_PRINT_ERR(ERRMSG_TOO_FEW "\n");
        err = true;
    }

    if (err)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        return 1;
    }

    return 0;
}


int _ParseArgs(int argc, char* argv[], std::string& _1, std::string& _2)
{
    int opt;
    int arg_cnt = 0;
    int opt_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, ""))
    {
        if (opterr != 0)
        {
            EXEC_PRINT_ERR("Argument error: %s\n", optmsg);
            err = true;
            resetopt();
            break;
        }
        if (opterr != 0)
        {
            EXEC_PRINT_ERR("Argument error: %s\n", optmsg);
            err = true;
            resetopt();
            break;
        }
        switch (opt)
        {
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
                _1 = optarg;
            else if (arg_cnt == 2)
                _2 = optarg;
            else if (arg_cnt == 3)
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

    if (arg_cnt < 2)
    {
        EXEC_PRINT_ERR(ERRMSG_TOO_FEW "\n");
        err = true;
    }

    if (err)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        return 1;
    }

    return 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Common operations.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int _ShowItem(XMLElementPtr node, bool detail, const char* key, WORD color)
{
    EntryList list;
    if (!PashDocUtil::GetEntries(node, list))
    {
        EXEC_PRINT_ERR("I... I can't see it!\n"); // :P
        return 1;
    }

    if (list.empty())
    {
        cnsl::InsertHeaderLine("Nothing", ' ');
        return 0; // return to reduce following indent
    }

    int total = 100;
    int maxKey = 0;
    int maxValue = 0;
    int maxWeight = 0;
    for (auto& it : list)
    {
        maxKey = std::max(maxKey, static_cast<int>(strlen(it.key)));
        maxValue = std::max(maxValue, static_cast<int>(strlen(it.value)));
    }
    maxKey = std::max(maxKey, 20);
    maxValue = std::max(maxValue, 20);
    maxWeight = std::min(total - maxKey - maxValue, 12);
    cnsl::InsertText(MESSAGE_COLOR, "     %4s | %*s | %*s | %*s\n",
                     "ID",
                     maxKey, "Key",
                     maxValue, "Value",
                     maxWeight, "Weight");

    int id = 0;
    auto hidden = "******";
    const char* value;
    constexpr WORD ENTRY_COLOR[2] = {FOREGROUND_WHITE, FOREGROUND_LIGHT(FOREGROUND_WHITE)};
    for (auto& it : list)
    {
        value = (!detail && _IsSensitive(it.key)) ? hidden : it.value;
        if (id < VAR_SIZE)
            cnsl::InsertText(VAR_COLOR, " $%d) ", id);
        else
            cnsl::InsertText("     ");

        if (key && _STR_SAME(it.key, key))
        {
            cnsl::InsertText(color, "%4d | %*s | %*s | %*d\n",
                             id,
                             maxKey, it.key,
                             maxValue, value,
                             maxWeight, it.weight);
        }
        else
        {
            cnsl::InsertText(ENTRY_COLOR[id & 1], "%4d | %*s | %*s | %*d\n",
                             id,
                             maxKey, it.key,
                             maxValue, value,
                             maxWeight, it.weight);
        }

        // always copy real value.
        if (id < VAR_SIZE)
            g_var[id] = it.value;
        id++;
    }

    return 0;
}


int _ShowItemSimple(XMLElementPtr node, bool detail, const char* key, WORD color)
{
    EntryList list;
    if (!PashDocUtil::GetEntries(node, list))
    {
        EXEC_PRINT_ERR("I... I can't see it!\n"); // :P
        return 1;
    }

    if (list.empty())
    {
        cnsl::InsertHeaderLine("Nothing", ' ');
        return 0; // return to reduce following indent
    }

    int total = 100;
    int maxKey = 0;
    int maxValue = 0;
    int maxWeight = 0;
    for (auto& it : list)
    {
        maxKey = std::max(maxKey, static_cast<int>(strlen(it.key)));
        maxValue = std::max(maxValue, static_cast<int>(strlen(it.value)));
    }
    maxKey = std::max(maxKey, 20);
    maxValue = std::max(maxValue, 20);
    maxWeight = std::min(total - maxKey - maxValue, 12);
    cnsl::InsertText(MESSAGE_COLOR, "%4s | %*s | %*s | %*s\n",
                     "ID",
                     maxKey, "Key",
                     maxValue, "Value",
                     maxWeight, "Weight");

    int id = 0;
    auto hidden = "******";
    const char* value;
    constexpr WORD ENTRY_COLOR[2] = {FOREGROUND_WHITE, FOREGROUND_LIGHT(FOREGROUND_WHITE)};
    for (auto& it : list)
    {
        value = (!detail && _IsSensitive(it.key)) ? hidden : it.value;
        if (key && _STR_SAME(it.key, key))
        {
            cnsl::InsertText(color, "%4d | %*s | %*s | %*d\n",
                             id,
                             maxKey, it.key,
                             maxValue, value,
                             maxWeight, it.weight);
        }
        else
        {
            cnsl::InsertText(ENTRY_COLOR[id & 1], "%4d | %*s | %*s | %*d\n",
                             id,
                             maxKey, it.key,
                             maxValue, value,
                             maxWeight, it.weight);
        }
        id++;
    }

    return 0;
}

bool _IsSensitive(const char* descr)
{
    static const char* const SENSITIVE_PATTERN[] = {
        ".*password.*",
        ".*pwd.*",
        ".*pin.*"
    };
    static constexpr int SENSITIVE_CNT = 3;

    std::regex pattern;

    for (int i = 0; i < SENSITIVE_CNT; i++)
    {
        pattern.assign(SENSITIVE_PATTERN[i], std::regex::icase);
        if (std::regex_match(descr, pattern))
            return true;
    }

    return false;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Password operations.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// fill password with '\0'. 
void _FormatPassword(char* password)
{
    char* p;
    for (p = password; *p; p++)
        continue;
    while (p - password < PASSWORD_MAX_LENGTH)
        *(p++) = '\0';
    *p = '\0';
}

void _FormatPassword(const char* buffer, char* password)
{
    memset(password, 0, sizeof(char) * PASSWORD_MAX_LENGTH);
    char* q = password;

    for (const char* p = buffer; *p; p++)
        *(q++) = *p;
    while (q - password < PASSWORD_MAX_LENGTH)
        *(q++) = '\0';
    *q = '\0';
}

void _HashPassword(char* hashPass)
{
    hash::MD5 md5;

    md5.Add(hashPass, strlen(hashPass));
    md5.GetHash((unsigned char*)hashPass);

    // hashPass[PASSWORD_MAX_LENGTH] = '\0';
}

void _HashPassword(const char* password, char* hashPass)
{
    hash::MD5 md5;

    md5.Add(password, strlen(password));
    md5.GetHash((unsigned char*)hashPass);

    // hashPass[PASSWORD_MAX_LENGTH] = '\0';
}
