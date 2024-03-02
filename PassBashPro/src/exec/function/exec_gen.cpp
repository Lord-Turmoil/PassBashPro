/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_gen.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 1, 2023                              *
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
#include "../../../inc/utility/Auxiliary.h"

/********************************************************************
** gen [-l 8] [-n 5] [-a -d -c]
*/

static constexpr int MIN_LENGTH = 3;
static constexpr int MAX_LENGTH = 32;

static constexpr int DEFAULT_LENGTH_MIN = 6;
static constexpr int DEFAULT_LENGTH_MAX = 12;

static const int MAX_TOP = VAR_SIZE;
static const int DEFAULT_TOP = std::min(VAR_SIZE, 3);

static bool hasLength;
static int length; // length of password
static int top; // generate how many results

static bool useDefault;
static bool hasAlpha;
static bool hasDigit;
static bool hasCharacter;

static constexpr char ALPHA_SET[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
static constexpr char DIGIT_SET[] = "0123456789";
static constexpr char CHARA_SET[] = "~!@#$%^&*()[]{}-_=+";

static std::vector<std::string> results;
static std::string candidates;

static void _gen_init();
static int _gen_usage();
static int _gen_parse_args(int argc, char* argv[]);

static int _init_generation();
static std::string _generate_password();

static int _print_result();


int exec_gen(int argc, char* argv[])
{
    _gen_init();
    if (_gen_parse_args(argc, argv) != 0)
    {
        _gen_usage();
        return 1;
    }

    _init_generation();
    for (int i = 0; i < top; i++)
        results.emplace_back(_generate_password());
    _print_result();

    return 0;
}


static void _gen_init()
{
    length = 0;
    hasLength = false;

    top = DEFAULT_TOP;

    useDefault = true;
    hasAlpha = false;
    hasDigit = false;
    hasCharacter = false;

    results.clear();
    candidates.clear();
}


static int _gen_usage()
{
    return ExecHost::GetInstance()
            ->execl(EXEC_GLOBAL, "help", "help", "generate", nullptr);
}


// gen [-l 8] [-n 5] [-a -d -c]
static int _gen_parse_args(int argc, char* argv[])
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, "l:n:adc"))
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
        case 'l':
            if (!ParseValue(optarg, &length))
            {
                err = true;
                EXEC_PRINT_ERR("Invalid value for '-l'!\n");
                break;
            }
            length = std::max(length, 1);
            length = std::min(length, MAX_LENGTH);
            hasLength = true;
            break;
        case 'n':
            if (!ParseValue(optarg, &top))
            {
                err = true;
                EXEC_PRINT_ERR("Invalid value for '-n'!\n");
                break;
            }
            top = std::max(top, 1);
            top = std::min(top, MAX_TOP);
            break;
        case 'a':
            hasAlpha = true;
            useDefault = false;
            break;
        case 'd':
            hasDigit = true;
            useDefault = false;
            break;
        case 'c':
            hasCharacter = true;
            useDefault = false;
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
            err = true;
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


static int _init_generation()
{
    if (useDefault)
    {
        hasAlpha = true;
        hasDigit = true;
        hasCharacter = false;
    }

    if (hasAlpha)
        candidates.append(ALPHA_SET);
    if (hasDigit)
        candidates.append(DIGIT_SET);
    if (hasCharacter)
        candidates.append(CHARA_SET);

    // shouldn't be empty
    if (candidates.empty())
        return 1;

    return 0;
}


static std::string _generate_password()
{
    std::string password;

    if (!hasLength)
        length = Random(DEFAULT_LENGTH_MIN, DEFAULT_LENGTH_MAX + 1);

    for (int i = 0; i < length; i++)
        password.push_back(candidates[Random(static_cast<int>(candidates.length()))]);

    return password;
}


static int _print_result()
{
    constexpr WORD ENTRY_COLOR[2] = { FOREGROUND_WHITE, FOREGROUND_LIGHT(FOREGROUND_WHITE) };

    for (int i = 0; i < results.size(); i++)
    {
        if (i < VAR_SIZE)
            cnsl::InsertText(VAR_COLOR, " $%d) ", i);
        else
            cnsl::InsertText("     ");

        cnsl::InsertText(ENTRY_COLOR[i & 1], "%s\n", results[i].c_str());

        if (i < VAR_SIZE)
            g_var[i] = results[i];
    }

    return 0;
}
