/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_archive.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 19, 2023                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Archive file into git repo.                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"

 // archive -o -r src
static bool _remove;
static bool _out;
static std::string _src;
static std::string _dst;

static void _archive_init();
static int _archive_usage();
static int _archive_parse_args(int argc, char* argv[]);

static int _archive_archive(bool keepOriginal);
static int _archive_output(bool keepOriginal);

static int _archive_move(const char* dst, const char* src, bool keepOriginal = false);
static int _archive_list();

int srv_archive(int argc, char* argv[])
{
    PASH_PANIC_ON(g_env == nullptr);

    _archive_init();

    if (argc == 1)
    {
        return _archive_list();
    }

    if (_archive_parse_args(argc, argv) != 0)
    {
        _archive_usage();
        return 1;
    }

    int ret;
    if (_out)
    {
        ret = _archive_output(!_remove);
        if (ret == 0)
        {
            EXEC_PRINT_MSG("Successfully extracted \'%s\' to \'%s\'!\n", _src.c_str(), _dst.c_str());
        }
        else
        {
            EXEC_PRINT_ERR("Failed to extract \'%s\' - %d\n!", _src.c_str(), ret);
            return ret;
        }
    }
    else
    {
        ret = _archive_archive(!_remove);
        if (ret == 0)
        {
            EXEC_PRINT_MSG("Successfully archived \'%s\' to \'%s\'!\n", _src.c_str(), _dst.c_str());
        }
        else
        {
            EXEC_PRINT_ERR("Failed to archive \'%s\' - %d!\n", _src.c_str(), ret);
            return ret;
        }
    }

    return 0;
}

// move file in
static int _archive_archive(bool keepOriginal)
{
    if (!FileUtil::Exists(_src.c_str()))
    {
        EXEC_PRINT_ERR("Source file doesn't exists!\n");
        return 11;
    }

    if (_dst == "config" || _dst == "data")
    {
        EXEC_PRINT_ERR("Illegal target name\n");
        return 12;
    }

    std::string root = g_env->rootPath;
    std::string dstPath(root);
    if (dstPath.back() != '\\')
    {
        dstPath.push_back('\\');
    }
    dstPath.append(_dst);

    return _archive_move(dstPath.c_str(), _src.c_str(), keepOriginal);
}

static int _archive_output(bool keepOriginal)
{
    if (_src == "config" || _src == "data" || _src[0] == '.')
    {
        EXEC_PRINT_ERR("Illegal target name\n");
        return 22;
    }

    std::string root = g_env->rootPath;
    std::string srcPath(root);
    if (srcPath.back() != '\\')
    {
        srcPath.push_back('\\');
    }
    srcPath.append(_src);

    if (!FileUtil::Exists(srcPath.c_str()))
    {
        EXEC_PRINT_ERR("Source file doesn't exists!\n");
        return 21;
    }

    return _archive_move(_dst.c_str(), srcPath.c_str(), keepOriginal);
}

// For now, just assume that the parent directory must exists.
static int _archive_move(const char* dst, const char* src, bool keepOriginal)
{
    if (FileUtil::Exists(dst))
    {
        char response[4];
        cnsl::InsertText(MESSAGE_COLOR, "Output file already exists, overwrite? (Y/N)\n");
        cnsl::InsertText(PROMPT_COLOR, "$ ");
        GetString(response, cnsl::InputOptions(1, 1));
        cnsl::InsertNewLine();
        if (tolower(response[0]) != 'y')
            return 0; // abort
    }
    bool ret;
    if (keepOriginal)
    {
        ret = FileUtil::MoveFileToNew(src, dst);
    }
    else
    {
        // default overwrite is true
        ret = FileUtil::CopyFileToNew(src, dst);
    }
    return ret ? 0 : 31;
}

static int _archive_list()
{
    std::vector<std::string> names;
    if (!FileUtil::GetFiles(g_env->rootPath.c_str(), nullptr, &names))
    {
        return 41;
    }

    int i = 0;
    for (auto& name : names)
    {
        if (name == "config" || name == "data" || name[0] == '.')
        {
            continue;
        }

        if (i < VAR_SIZE)
        {
            cnsl::InsertText(VAR_COLOR, " $%d) ", i);
        }
        else
        {
            cnsl::InsertText("     ");
        }

        cnsl::InsertText("%s\n", name.c_str());

        if (i < VAR_SIZE)
        {
            g_var[i++] = name;
        }
    }
    if (i == 0)
    {
        EXEC_PRINT_MSG("Nothing...\n");
    }

    return 0;
}

static void _archive_init()
{
    _out = false;
    _remove = false;
}

static int _archive_usage()
{
    return ExecHost::GetInstance()
        ->execl(EXEC_GLOBAL, "help", "help", "archive", nullptr);
}

static int _archive_parse_args(int argc, char* argv[])
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, "or"))
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
        case 'o':
            _out = true;
            break;
        case 'r':
            _remove = true;
            break;
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
            {
                _src = optarg;
            }
            else if (arg_cnt == 2)
            {
                _dst = optarg;
            }
            else if (arg_cnt == 3)
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

    if (_out && _remove)
    {
        // only one
        err = true;
        EXEC_PRINT_ERR("Cannot assign both -e and -o!");
    }
    if (arg_cnt < 2)
    {
        err = true;
        EXEC_PRINT_ERR(ERRMSG_TOO_FEW "\n");
    }
    if (err)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        return 1;
    }

    return 0;
}
