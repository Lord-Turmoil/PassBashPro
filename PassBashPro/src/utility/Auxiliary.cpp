/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Auxiliary.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Auxiliary functions.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/common/Constants.h"
#include "../../inc/utility/Auxiliary.h"

#include "../../inc/utility/xml.h"

#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include <Windows.h>


char* strstrip(char* str)
{
    if (!str)
        return nullptr;

    char* base = str;
    char* left = base;
    char* right = base + strlen(str) - 1;

    if (left <= right)
    {
        while ((left <= right) && isspace(*left))
            left++;
        while ((left <= right) && isspace(*right))
            right--;

        for (char* p = left; p <= right; p++)
            *(base++) = *p;
        *base = '\0';
    }

    return str;
}

char* strtolower(char* str)
{
    if (!str)
        return nullptr;

    for (char* p = str; *p; p++)
        *p = tolower(*p);

    return str;
}

char* strtoupper(char* str)
{
    if (!str)
        return nullptr;

    for (char* p = str; *p; p++)
        *p = toupper(*p);

    return str;
}


bool is_null_or_empty(const char* str)
{
    return !str || !*str;
}

bool begins_with(const char* str, const char* prefix)
{
    if (is_null_or_empty(prefix))
        return true;
    if (is_null_or_empty(str))
        return false;

    const char* original_str = str;
    while (*prefix && *str)
    {
        if (*prefix != *str)
            return false;
        prefix++;
        str++;
    }

    return !*prefix;
}

bool ends_with(const char* str, const char* suffix)
{
    if (is_null_or_empty(suffix))
        return true;
    if (is_null_or_empty(str))
        return false;

    const char* str_end = str + strlen(str);
    const char* suffix_end = suffix + strlen(suffix);

    while (str_end >= str && suffix_end >= suffix)
    {
        if (*str_end != *suffix_end)
            return false;
        str_end--;
        suffix_end--;
    }

    return suffix_end < suffix;
}


/******************************************************************************
 * widen -- Widen char to wchar_t.                                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   dest -- The destination wchar_t buffer.                           *
 *          src  -- The source char buffer.                                   *
 *                                                                            *
 * OUTPUT:  Return whether successful or not.                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/02/13 Tony : Created.                                               *
 *============================================================================*/
bool widen(wchar_t* dest, const char* src)
{
    size_t ret;
    errno_t err = mbstowcs_s(&ret, dest, strlen(src) + 1, src, PASH_BUFFER_SIZE);

    return err ? false : true;
}


/******************************************************************************
 * widen -- Widen char to wchar_t.                                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   src  -- The source char buffer.                                   *
 *                                                                            *
 * OUTPUT:  Return wchar_t string. nullptr if conversion failed.              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/02/13 Tony : Created.                                               *
 *============================================================================*/
wchar_t* widen(const char* src)
{
    static wchar_t dest[PASH_BUFFER_SIZE];

    size_t ret;
    errno_t err = mbstowcs_s(&ret, dest, strlen(src) + 1, src, PASH_BUFFER_SIZE);

    return err ? nullptr : dest;
}


/******************************************************************************
 * narrow -- Narrow wchar_t to char.                                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   dest -- The destination char buffer.                              *
 *          src  -- The source char buffer.                                   *
 *                                                                            *
 * OUTPUT:  Return whether successful or not.                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/12 Tony : Created.                                               *
 *============================================================================*/
bool narrow(char* dest, const wchar_t* src)
{
    size_t ret;
    errno_t err = wcstombs_s(&ret, dest, wcslen(src) + 1, src, PASH_BUFFER_SIZE);

    return err ? false : true;
}


/******************************************************************************
 * narrow -- Narrow wchar_t to char.                                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   src  -- The source wchar_t buffer.                                *
 *                                                                            *
 * OUTPUT:  Return char string. nullptr if conversion failed.                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/12 Tony : Created.                                               *
 *============================================================================*/
char* narrow(const wchar_t* src)
{
    static char dest[PASH_BUFFER_SIZE];

    size_t ret;
    errno_t err = wcstombs_s(&ret, dest, wcslen(src) + 1, src, PASH_BUFFER_SIZE);

    return err ? nullptr : dest;
}


const char* GetCurrentTimestamp()
{
    static char _time_buffer[64];

    SYSTEMTIME sysTime;

    GetLocalTime(&sysTime);

    //YYYY-MM-DD HH-MM-SS
    sprintf_s(_time_buffer, "%4hu-%02hu-%02hu %02hu-%02hu-%02hu",
              sysTime.wYear, sysTime.wMonth, sysTime.wDay,
              sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

    return _time_buffer;
}

/******************************************************************************
 * ResetRandomSeed -- Reset random seed.                                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void SetRandomSeed()
{
    srand(static_cast<unsigned>(time(NULL)));
}

/******************************************************************************
 * Random -- Get a random number.                                             *
 *                                                                            *
 *    Get a random number in [lower, upper).                                  *
 *                                                                            *
 * INPUT:   lower -- If not assigned, the lower will be zero.                 *
 *          upper -- The upper bound of the result. upper not included.       *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
int Random(int upper)
{
    return (upper == 0) ? (0) : (rand() % upper);
}

int Random(int lower, int upper)
{
    if (upper <= lower)
        return lower;

    return lower + Random(upper - lower);
}


int ParseValue(const char* str, int* val)
{
    return tinyxml2::XMLUtil::ToInt(str, val);
}
