/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_help.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   None                                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"
#include "../../../inc/utility/FileUtil.h"

#include <algorithm>


static std::string target;
static bool showAll;
static bool showDetail;
static char _help_buffer[PASH_BUFFER_SIZE];

static int _help_usage();
static int _help_parse_args(int argc, char* argv[]);

// help single command (in detail)
static int _help_single(const std::string& item, bool brief = false);
// help current faction (in brief)
static int _help_current(const char* root = nullptr);
// help all factions (in brief)
static int _help_all();

static int _print_help(const char* path, bool brief);

static bool _is_empty_line(const char* line);

int exec_help(int argc, char* argv[])
{
    target = "";
    showAll = false;
    showDetail = false;

    if (_help_parse_args(argc, argv) != 0)
    {
        // Prevent potential recursive call.
        if (argc > 0 && argv[0][0] != '_')
            _help_usage();
    }

    std::transform(target.begin(), target.end(), target.begin(),
                   [](char c) { return tolower(c); });

    if (showAll)
        return _help_all();
    if (target == "")
        return _help_current();
    PASH_TRY(_help_single(target, !showDetail));
    if (!showDetail)
        EXEC_PRINT_MSG("Use -d parameter to learn detailed information.\n");
    return 0;
}

static int _help_usage()
{
    // Use '_help' to differentiate internal and external call.
    return ExecHost::GetInstance()->execl(EXEC_GLOBAL, "help", "_help", "help", nullptr);
}

static int _help_parse_args(int argc, char* argv[])
{
    int opt;
    int arg_cnt = 0;
    int opt_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, "ad"))
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
        case 'a':
            // temporarily disable this feature
            // showAll = true;
            break;
        case 'd':
            showDetail = true;
            break;
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
            {
                // optarg might be a const char*!
                target = optarg;
            }
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


static int _help_single(const std::string& item, bool brief) // help single command (in detail)
{
    std::string path(HELP_ROOT_DIR);
    path.append(HELP_DIR[g_mode]);
    path.append(item);
    if (!ends_with(path.c_str(), ".txt"))
        path.append(".txt");

    int ret = _print_help(path.c_str(), brief);
    if (ret != 0)
    {
        EXEC_PRINT_ERR("Help for '%s' is not available!\n", item.c_str());
        return 1;
    }

    if (!brief)
        cnsl::InsertNewLine();

    return 0;
}

static int _help_current(const char* root) // help current faction (in brief)
{
    std::string path;

    if (root)
        path.assign(root);
    else
        path.assign(HELP_ROOT_DIR).append(HELP_DIR[g_mode]);

    std::vector<std::string> names;
    FileUtil::GetFiles(path.c_str(), nullptr, &names);

    std::string faction;
    PashDocUtil::GetBaseName(path, faction);
    cnsl::InsertHeaderLine(faction.c_str(), '-');
    if (names.empty())
        EXEC_PRINT_MSG("No available help information.\n");
    else
    {
        for (auto name : names)
        {
            _help_single(name, true);
            cnsl::InsertNewLine();
        }
    }

    return 0;
}

static int _help_all() // help all factions (in brief)
{
    std::string path(HELP_ROOT_DIR);
    std::vector<std::string> dirs;
    int ret = 0;

    FileUtil::GetDirectories(path.c_str(), &dirs, nullptr);
    if (dirs.empty())
        EXEC_PRINT_MSG("No available help information.\n");
    else
    {
        std::string faction;
        for (auto dir : dirs)
        {
            if (_help_current(dir.c_str()) != 0)
                ret++;
        }
    }

    return ret;
}

static bool _is_empty_line(const char* line)
{
    for (const char* p = line; *p; p++)
    {
        if (!isspace(*p))
            return false;
    }

    return true;
}

static int _print_help(const char* path, bool brief)
{
    FILE* fp;

    if (fopen_s(&fp, path, "r") != 0)
        return 1;

    WORD old = cnsl::SetTextForeground(MESSAGE_COLOR);

    if (brief)
    {
        while (fgets(_help_buffer, PASH_BUFFER_SIZE, fp) != nullptr)
        {
            if (_is_empty_line(_help_buffer))
                break;
            cnsl::InsertText("%s", _help_buffer);
        }
    }
    else
    {
        cnsl::InsertText("Usage:\n");
        while (fgets(_help_buffer, PASH_BUFFER_SIZE, fp) != nullptr)
            cnsl::InsertText("%s", _help_buffer);
    }

    cnsl::SetTextForeground(old);

    fclose(fp);

    return 0;
}
