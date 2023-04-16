/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_copy.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 16, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Copy arguments to clipboard.                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"
#include "../../../inc/utility/Auxiliary.h"

#include <cstring>


int _copy(const std::string& data);
int exec_copy(int argc, char* argv[])
{
    std::string data;

    for (int i = 1; i < argc; i++)
    {
        if (!is_null_or_empty(argv[i]))
        {
            if (!data.empty())
                data.append(" ");
            data.append(argv[i]);
        }
    }

    if (data.empty())
    {
        EXEC_PRINT_MSG("Nothing to copy...\n");
        return 0;
    }

    if (_copy(data) != 0)
    {
        EXEC_PRINT_ERR("Something went error when copy content to clipboard.\n");
        return 1;
    }

    EXEC_PRINT_MSG("Successfully copied content to clipboard!\n");

    return 0;
}

int _copy(const std::string& data)
{
    HWND hWnd = nullptr;
    
    OpenClipboard(hWnd);
    EmptyClipboard();
    
    size_t size = data.length() + 4;

    HANDLE hHandle = GlobalAlloc(GMEM_FIXED, data.length() + 4);
    if (!hHandle)
        return 1;
    
    char* pData = (char*)GlobalLock(hHandle);//锁定内存，返回申请内存的首地址
    if (!pData)
        return 2;
    strcpy_s(pData, size, data.c_str());

    SetClipboardData(CF_TEXT, hHandle);
    GlobalUnlock(hHandle);
    CloseClipboard();

    return 0;
}
