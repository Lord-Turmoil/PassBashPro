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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/utility/Auxiliary.h"
#include "../../inc/utility/FileUtil.h"
#include "../../inc/common/Constants.h"
#include "../../inc/common/Logger.h"

#include <Windows.h>
#include <io.h>     // for _access
#include <direct.h>	// for _mkdir
#include <string>

static char buffer[PASH_BUFFER_SIZE];

bool FileUtil::Exists(const char* path)
{
	return (_access(path, 0) == 0);
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
			else // EEXIST
			{
				// LOG_MESSAGE("Directory \'%s\' already exists", curPath.c_str());
			}
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
		if (fileInfo.attrib & _A_SUBDIR)	// is a directory
		{
			p.assign(base);
			p.append("\\").append(fileInfo.name).append("\\");
			if (type & _DIR)
			{
				if (paths)
					paths->push_back(p);
				if (names)
					names->push_back(fileInfo.name);
			}
			_GetContent(p.c_str(), paths, names, type);
		}
		else
		{
			p.assign(base);
			p.append("\\");
			p.append(fileInfo.name);
			if (paths)
				paths->push_back(p);
			if (names)
				names->push_back(fileInfo.name);
		}
	} while (_findnext(hFile, &fileInfo) == 0);
}
