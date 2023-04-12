/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : ServiceAux.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   None                                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceAux.h"
#include "../../../inc/exec/ExecHeader.h"
#include "../../../inc/utility/FileUtil.h"

#include <tea.h>


EnvPtr CreateEnv(Profile* profile)
{
	if (!profile)
		return nullptr;

	EnvPtr env(new Env());

	env->username.assign(profile->username);
	strcpy_s(env->password, DEFAULT_PASSWORD);
	env->rootPath.assign(profile->path);
	env->dataPath.assign(profile->path).append(DATA_FILE);
	env->configPath.assign(profile->path).append(CONFIG_FILE);
	env->profilePath.assign(profile->path).append(PROFILE_FILE);

	return env;
}


int InitConfig(EnvPtr env)
{
	const char* configPath = env->configPath.c_str();
	
	if (FileUtil::Exists(configPath))
		return 0;

	if (!FileUtil::NewDirectory(env->rootPath.c_str()))
		return 1;

	FILE* output;
	if (fopen_s(&output, configPath, "wb") != 0)
	{
		LOG_ERROR("Failed to open file \"%s\"", configPath);
		return 2;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(env->password);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, env->password);
	delete reader;
	delete writer;

	return 0;
}

int InitData(EnvPtr env)
{
	const char* dataPath = env->dataPath.c_str();

	if (FileUtil::Exists(dataPath))
		return 0;

	if (!FileUtil::NewDirectory(env->rootPath.c_str()))
		return 1;

	FILE* output;
	if (fopen_s(&output, dataPath, "wb") != 0)
	{
		LOG_ERROR("Cannot open file \"%s\"", dataPath);
		return 2;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(DEFAULT_DATA);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, env->password);
	delete reader;
	delete writer;

	return 0;
}


int SaveConfig(EnvPtr env, bool overwrite)
{
	if (!FileUtil::NewDirectory(env->rootPath.c_str()))
		return 1;

	if (!overwrite && FileUtil::Exists(env->configPath.c_str()))
		return 0;

	FILE* output;
	const char* configPath = env->configPath.c_str();

	if (fopen_s(&output, configPath, "wb") != 0)
	{
		LOG_ERROR("Failed to open file \"%s\"", configPath);
		return 2;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(env->password);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, env->password);
	delete reader;
	delete writer;

	return 0;
}

int SaveData(PashDoc& doc, EnvPtr env, bool overwrite)
{
	if (!FileUtil::NewDirectory(env->rootPath.c_str()))
		return 1;

	if (!overwrite && FileUtil::Exists(env->dataPath.c_str()))
		return 0;

	if (!doc.IsLoaded())
	{
		LOG_ERROR("Save before load");
		return 2;
	}

	doc.Save(env);

	return 0;
}

int InitEnvFiles(EnvPtr env)
{
	int ret;
	PashDoc doc;

	ret = InitConfig(env);
	if (ret != 0)
	{
		LOG_ERROR("Failed to initialize config: %d", ret);
		return 1;
	}

	ret = InitData(env);
	if (ret != 0)
	{
		LOG_ERROR("Failed to initialize data: %d", ret);
		return 2;
	}

	return 0;
}
