/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_tea.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 16, 2023                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For tea encryption util.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"
#include "../../../inc/utility/FileUtil.h"

#include <tea.h>


static int _mode; //  0 for encrypt and 1 for decrypt
static std::string _password;
static char _buffer[PASSWORD_BUFFER_SIZE];

static std::string _src;
static std::string _dst;

static void _tea_init();
static int _tea_usage();
static int _tea_parse_args(int argc, char* argv[]);
static int _open_files(FILE** pfin, const char* src, FILE** pfout, const char* dst);
static void _close_files(FILE** fin, FILE** fout);
static int _enctypt(FILE* fin, FILE* fout);

int exec_tea(int argc, char* argv[])
{
    _tea_init();
    if (_tea_parse_args(argc, argv) != 0)
    {
        _tea_usage();
        return 1;
    }

    if (_src == _dst)
    {
        EXEC_PRINT_ERR("Input and output must not be the same!\n");
        return 2;
    }

    // initialize password
    _HashPassword(_password.c_str(), _buffer);

    // open files
    FILE* fin = nullptr;
    FILE* fout = nullptr;

    int ret = _open_files(&fin, _src.c_str(), &fout, _dst.c_str());
    if (ret > 0)
    {
        _close_files(&fin, &fout);

        cnsl::InsertText(ERROR_COLOR, "Failed to open files!\n");
        cnsl::InsertText(MESSAGE_COLOR, "Tips: You may need absolute path.\n");

        return 3;
    }
    if (ret == -1)
    {
        _close_files(&fin, &fout);
        cnsl::InsertText(MESSAGE_COLOR, "Encryption aborted.\n");
        return 0;
    }

    if (_enctypt(fin, fout) != 0)
    {
        cnsl::InsertText(ERROR_COLOR, "Failed to %s file.\n", (_mode == 0) ? "encrypt" : "decrypt");
        _close_files(&fin, &fout);
        return 4;
    }
    fin = fout = nullptr;

    cnsl::InsertText(MESSAGE_COLOR, "%s complete.\n", (_mode == 0) ? "Encryption" : "Decryption");

    // if succeeded, fin and fout will be closed by tea reader and writer.

    return 0;
}

static void _tea_init()
{
    _mode = 0;
    _password = "";
    _src = _dst = "";
}

static int _tea_usage()
{
    return ExecHost::GetInstance()
        ->execl(EXEC_GLOBAL, "help", "help", "tea", nullptr);
}

static int _tea_parse_args(int argc, char* argv[])
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, "dp:"))
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
        case 'd':
            _mode = 1; // decrypt
            break;
        case 'p':
            _password = optarg;
            break;
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
                _src = optarg;
            else if (arg_cnt == 2)
                _dst = optarg;
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

    if (arg_cnt < 2)
    {
        err = true;
        EXEC_PRINT_ERR(ERRMSG_TOO_FEW "\n");
    }
    if (_password == "")
    {
        err = true;
        EXEC_PRINT_ERR("Missing password!\n");
    }
    if (err)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        return 1;
    }

    return 0;
}

static int _open_files(FILE** pfin, const char* src, FILE** pfout, const char* dst)
{
    if (FileUtil::Exists(dst))
    {
        char response[4];
        cnsl::InsertText(MESSAGE_COLOR, "Output file already exists, overwrite? (Y/N)\n");
        cnsl::InsertText(PROMPT_COLOR, "$ ");
        GetString(response, cnsl::InputOptions(1, 1));
        cnsl::InsertNewLine();
        if (tolower(response[0]) != 'y')
            return -1; // abort
    }

    if (fopen_s(pfin, src, "rb") != 0)
    {
        cnsl::InsertText(ERROR_COLOR, "Could not open input file!\n");
        return 2;
    }
    if (fopen_s(pfout, dst, "wb") != 0)
    {
        cnsl::InsertText(ERROR_COLOR, "Could not open output file!\n");
        return 3;
    }

    return 0;
}

static void _close_files(FILE** fin, FILE** fout)
{
    if (fin && *fin)
    {
        fclose(*fin);
        *fin = nullptr;
    }
    if (fout && *fout)
    {
        fclose(*fout);
        *fout = nullptr;
    }
}

static int _enctypt(FILE* fin, FILE* fout)
{
    auto reader = new tea::TEAFileReader(fin);
    auto writer = new tea::TEAFileWriter(fout);
    if (_mode == 0)
        encode(reader, writer, _buffer);
    else if (_mode == 1)
        decode(reader, writer, _buffer);
    else
        return 1;
    delete reader;
    delete writer;

    return 0;
}
