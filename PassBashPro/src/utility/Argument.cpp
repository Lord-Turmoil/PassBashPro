/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Argument.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For basic argument parsing.                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../inc/utility/Argument.h"

#include <cstdio>
#include <cstring>

static char _opt_buffer[128];

char* optarg;
const char* optmsg;
int opterr;
int optopt;

static int optind;

void resetopt()
{
    optind = 0;
    opterr = 0;
    optarg = nullptr;
    optmsg = nullptr;
}

static int _parseopt(const char* arg);

// parse arg in opt. e.g. pthread from -lpthread
// Must checked by _parseopt first.
static char* _parsearg(char* arg);

static void _initopt();

int getopt(int argc, char* argv[], const char* pattern)
{
    _initopt();

    if (optind >= argc)
    {
        resetopt();
        return 0;
    }

    int opt = _parseopt(argv[optind]);
    if (!opt) // not an option
    {
        opt = optopt = '!';
        optarg = argv[optind];
    }
    else // is an option
    {
        optopt = opt;
        auto pos = strchr(pattern, opt);
        if (!pos) // not valid option
        {
            opt = '?';
            opterr = ERRNO_INVALID_OPTION;
            sprintf_s(_opt_buffer, "Illegal argument %c", opt);
        }
        else
        {
            if (pos[1] == ':') // has argument
            {
                optarg = _parsearg(argv[optind]);
                if (!optarg) // argument not compact
                {
                    if (((optind < argc - 1) && _parseopt(argv[optind + 1])) || (optind == argc - 1))
                    {
                        opterr = ERRNO_MISSING_ARGUMENT;
                        sprintf_s(_opt_buffer, "Missing argument for parameter %c", opt);
                        optmsg = _opt_buffer;
                    }
                    else
                        optarg = argv[++optind];
                }
            }
        }
    }

    optmsg = _opt_buffer;

    return opt;
}

static int _parseopt(const char* arg)
{
    if (arg[0] == '-' && arg[1] != '\0')
        return arg[1];
    return 0;
}

// parse arg in opt. e.g. pthread from -lpthread
// Must checked by _parseopt first.
static char* _parsearg(char* arg)
{
    if (arg[2] != '\0')
        return &arg[2];
    return nullptr;
}

static void _initopt()
{
    optarg = nullptr;
    optmsg = nullptr;
    opterr = 0;
    optopt = '?';
    optind++;
}
