/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : srv_start.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The first service to be launched. This service is used to initialize all *
 * configurations and environment.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceHeader.h"
#include "../../../inc/core/Profile.h"


static int _ensure_env();
static int _load_env();
static int _load_profile();
static int _load_cache();
static int _login_init_env();

static char _cached_user[USERNAME_BUFFER_SIZE];

int srv_start(int argc, char* argv[])
{
	int ret = _load_env();
	if (ret != 0)
	{
		LOG_ERROR("Failed to load env: %d", ret);
		return 1;
	}

	if (!g_env)
	{
		// Use absolute zero to indicate internal call.
		ret = ExecHost::GetInstance()
			->execl(EXEC_SERVICE, "profile", nullptr);
		if (ret != 0)
		{
			if (ret == -TERMINATION)
				return 0;
			LOG_ERROR("Failed to create profile: %d", ret);
			return 2;
		}
	}
	if (!g_env)
	{
		LOG_ERROR("Env init failed");
		return 3;
	}

	ret = ExecHost::GetInstance()
		->execl(EXEC_SERVICE, "login", "login", nullptr);
	if (ret != 0)
	{
		if (ret == -TERMINATION)
			return 0;
		LOG_ERROR("Failed to login: %d", ret);
		return 4;
	}
	
	ret = ExecHost::GetInstance()
		->execl(EXEC_SERVICE, "host", "host", nullptr);
	if (ret != 0)
	{
		if (ret == -TERMINATION)
			return 0;
		LOG_ERROR("Error occurred in host: %d", ret);
		return 5;
	}

	return 0;
}

static int _ensure_env()
{
	if (!FileUtil::Exists(PASH_DIR))
	{
		FileUtil::NewDirectory(PASH_DIR, true);
	}

	return 0;
}

static int _load_env()
{
	_ensure_env();

	_load_profile();
	_load_cache();

	_login_init_env();

	return 0;
}

static int _load_profile()
{
	std::vector<std::string> dirs;
	std::vector<std::string> names;

	if (!FileUtil::GetDirectories(PASH_DIR, &dirs, &names))
	{
		return 1;
	}

	int size = dirs.size();
	if (size != names.size())
		return 2;

	ProfilePool* pool = ProfilePool::GetInstance();
	pool->Clear();
	for (int i = 0; i < size; i++)
		pool->Add(Profile(names[i], dirs[i]));
}

static int _load_cache()
{
	std::string path(PASH_DIR);
	path.append(CACHE_FILE);
	FILE* fp;
	ProfilePool* pool = ProfilePool::GetInstance();

	if (fopen_s(&fp, path.c_str(), "r") != 0)
	{
		if (!pool->IsEmpty())
		{
			fopen_s(&fp, path.c_str(), "w");
			fprintf(fp, "%s\n", (*pool)[0].username.c_str());
			fclose(fp);
		}

		if (fopen_s(&fp, path.c_str(), "r") != 0)
		{
			LOG_ERROR("Cannot create cache");
			return 1;
		}
	}

	// here, fp is open
	fscanf_s(fp, "%[^\n]", _cached_user);
	fclose(fp);
}

static int _login_init_env()
{
	ProfilePool* pool = ProfilePool::GetInstance();
	Profile* p;

	p = pool->Get(_cached_user);
	if (!p)
	{
		if (!pool->IsEmpty())
			p = &(*pool)[0];
	}

	if (!p)
		return 1;

	g_env = CreateEnv(p);

	return 0;
}
