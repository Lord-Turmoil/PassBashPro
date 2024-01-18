/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : FileUtil.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For file operations.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../inc/utility/Auxiliary.h"
#include "../../inc/utility/FileUtil.h"
#include "../../inc/common/Constants.h"
#include "../../inc/common/Logger.h"

#include <Windows.h>
#include <io.h>     // for _access
#include <direct.h>	// for _mkdir
#include <string>
#include <cstring>

static char buffer[PASH_BUFFER_SIZE];


bool FileUtil::Exists(const char* path)
{
    return (_access(path, 0) == 0);
}


bool FileUtil::Exists(const wchar_t* path)
{
    return (_waccess(path, 0) == 0);
}


bool FileUtil::NewFile(const char* path)
{
    // To be continued...

    return true;
}


bool FileUtil::NewFile(const char* path, const char* filename)
{
    std::string fullPath = path;
    if (!fullPath.empty() && (fullPath.back() != '/'))
        fullPath.push_back('/');
    fullPath.append(filename);

    return NewFile(fullPath.c_str());
}


bool FileUtil::NewDirectory(const char* path, bool hidden)
{
    char* context = nullptr;
    char* buffer = _strdup(path);

    char* token = strtok_s(buffer, "/", &context);
    if (!token)
    {
        LOG_ERROR("Invalid directory name!");
        return false;
    }

    std::string curPath("");
    while (token)
    {
        if (!curPath.empty())
            curPath += '\\';
        curPath += token;
        if (_mkdir(curPath.c_str()) != 0)
        {
            if (errno == ENOENT)
            {
                LOG_ERROR("Faild to create \'%s\'", curPath.c_str());
                free(buffer);
                return false;
            }
            // EEXIST
            // LOG_MESSAGE("Directory \'%s\' already exists", curPath.c_str());
        }
        token = strtok_s(nullptr, "\\/", &context);
    }

    if (hidden)
        SetFileAttributes(widen(curPath.c_str()), FILE_ATTRIBUTE_HIDDEN);

    return true;
}


bool FileUtil::NewDirectory(const char* path, const char* dirname, bool hidden)
{
    std::string fullPath = path;
    if (!fullPath.empty() && (fullPath.back() != '\\'))
        fullPath.push_back('\\');
    fullPath.append(dirname);

    return NewDirectory(fullPath.c_str(), hidden);
}


bool FileUtil::DeleteFilePath(const char* path)
{
    return DeleteFilePath(widen(path));
}


bool FileUtil::DeleteFilePath(const wchar_t* path)
{
    return DeleteFile(path);
}


bool FileUtil::DeletePath(const char* path)
{
    if (!Exists(path))
        return true;

    _DeleteDirectory(path);

    return true;
}


bool FileUtil::DeletePath(const wchar_t* path)
{
    if (!Exists(path))
        return true;

    wchar_t cmd[PASH_BUFFER_SIZE];
    swprintf_s(cmd, L"rmdir /S /Q \"%s\"", path);
    _wsystem(cmd);

    // _DeleteDirectory(path);

    return true;
}


bool FileUtil::CopyFileToNew(const char* src, const char* dst, bool overwrite)
{
    if (!Exists(src))
        return false;

    auto w_src = new wchar_t[strlen(src) + 4];
    auto w_dst = new wchar_t[strlen(dst) + 4];

    widen(w_src, src);
    widen(w_dst, dst);

    int ret = CopyFile(w_src, w_dst, !overwrite);

    delete[] w_src;
    delete[] w_dst;

    return ret;
}


bool FileUtil::MoveFileToNew(const char* src, const char* dst)
{
    if (!Exists(src))
        return false;

    auto w_src = new wchar_t[strlen(src) + 4];
    auto w_dst = new wchar_t[strlen(dst) + 4];

    widen(w_src, src);
    widen(w_dst, dst);

    int ret = MoveFile(w_src, w_dst);

    delete[] w_src;
    delete[] w_dst;

    return ret;
}


bool FileUtil::GetFiles(const char* path,
                        std::vector<std::string>* files,
                        std::vector<std::string>* names)
{
    return _GetContent(path, files, names, _FILE);
}


bool FileUtil::GetDirectories(const char* path,
                              std::vector<std::string>* dirs,
                              std::vector<std::string>* names)
{
    return _GetContent(path, dirs, names, _DIR);
}


bool FileUtil::GetContent(const char* path,
                          std::vector<std::string>* paths,
                          std::vector<std::string>* names)
{
    return _GetContent(path, paths, names, _ALL);
}


bool FileUtil::_GetContent(const char* path,
                           std::vector<std::string>* paths,
                           std::vector<std::string>* names,
                           FileType type)
{
    if (!Exists(path))
        return false;

    if (paths)
        paths->clear();
    if (names)
        names->clear();

    _GetContentAux(path, paths, names, type);

    return true;
}


void FileUtil::_GetContentAux(const char* path,
                              std::vector<std::string>* paths,
                              std::vector<std::string>* names,
                              FileType type)
{
    long long hFile = 0;
    struct _finddata_t fileInfo;

    std::string base(path);
    while (!base.empty() && (base.back() == '\\'))
        base.pop_back();
    std::string p = base + "\\*";

    hFile = _findfirst(p.c_str(), &fileInfo);
    if (hFile == -1)
        return;

    do
    {
        if (fileInfo.attrib & _A_SUBDIR) // is a directory
        {
            if (_STR_SAME(fileInfo.name, "."))
                continue;
            if (_STR_SAME(fileInfo.name, ".."))
                continue;

            p.assign(base);
            p.append("\\").append(fileInfo.name).append("\\");
            if (type & _DIR)
            {
                if (paths)
                    paths->push_back(p);
                if (names)
                    names->push_back(fileInfo.name);
            }
            // _GetContent(p.c_str(), paths, names, type);
        }
        else if (type & _FILE)
        {
            p.assign(base);
            p.append("\\");
            p.append(fileInfo.name);
            if (paths)
                paths->push_back(p);
            if (names)
                names->push_back(fileInfo.name);
        }
    }
    while (_findnext(hFile, &fileInfo) == 0);
}


void FileUtil::_DeleteDirectory(const char* path)
{
    long long hFile = 0;
    struct _finddata_t fileInfo;

    std::string base(path);
    while (!base.empty() && (base.back() == '\\'))
        base.pop_back();
    std::string p = base + "\\*";

    hFile = _findfirst(p.c_str(), &fileInfo);
    if (hFile == -1)
        return;

    do
    {
        if (fileInfo.attrib & _A_SUBDIR) // is a directory
        {
            if (_STR_SAME(fileInfo.name, "."))
                continue;
            if (_STR_SAME(fileInfo.name, ".."))
                continue;

            p.assign(base);
            p.append("\\").append(fileInfo.name).append("\\");

            _DeleteDirectory(p.c_str());
        }
        else
        {
            p.assign(base);
            p.append("\\");
            p.append(fileInfo.name);
            remove(p.c_str());
        }
    }
    while (_findnext(hFile, &fileInfo) == 0);

    _rmdir(path);
}


void FileUtil::_DeleteDirectory(const wchar_t* path)
{
    long long hFile = 0;
    struct _wfinddata_t fileInfo;

    std::wstring base(path);
    while (!base.empty() && (base.back() == '\\'))
        base.pop_back();
    std::wstring p = base + L"\\*";

    hFile = _wfindfirst(p.c_str(), &fileInfo);
    if (hFile == -1)
        return;

    do
    {
        if (fileInfo.attrib & _A_SUBDIR) // is a directory
        {
            if (_WSTR_SAME(fileInfo.name, L"."))
                continue;
            if (_WSTR_SAME(fileInfo.name, L".."))
                continue;

            p.assign(base);
            p.append(L"\\").append(fileInfo.name).append(L"\\");

            _DeleteDirectory(p.c_str());
        }
        else
        {
            p.assign(base);
            p.append(L"\\");
            p.append(fileInfo.name);
            _wremove(p.c_str());
        }
    }
    while (_wfindnext(hFile, &fileInfo) == 0);

    _wrmdir(path);
}
