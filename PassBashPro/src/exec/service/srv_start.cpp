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

#include <cstring>


static int _ensure_env();
static int _load_env();
static int _load_profile();
static int _load_cache();
static int _login_init_env();

static char _cached_user[USERNAME_BUFFER_SIZE];

int srv_start(int argc, char* argv[])
{
	if (ExecHost::GetInstance()
		->execl(EXEC_GLOBAL, "version", "version", nullptr) == 0)
	{
		cnsl::InsertNewLine();
	}

	int ret = _load_env();
	if (ret != 0)
	{
		LOG_ERROR("Failed to load env: %d", ret);
		return 1;
	}

	// first launch
	if (!g_env)
	{
		// Use absolute zero to indicate internal call.
		ret = ExecHost::GetInstance()
			->execl(EXEC_SERVICE, "profile", "profile", "-i", nullptr);
		if (ret != 0)
		{
			if (ret == TERMINATION)
				return 0;
			LOG_ERROR("Failed to create profile: %d", ret);
			return 2;
		}

		// double-check
		PASH_PANIC_ON(_load_env());
	}

	ret = ExecHost::GetInstance()
		->execl(EXEC_SERVICE, "login", "login", nullptr);
	while (ret == -1)	// To create new user.
	{
		ret = ExecHost::GetInstance()
			->execl(EXEC_SERVICE, "profile", "profile", "-i", nullptr);
		if (ret != 0)
		{
			if (ret == TERMINATION)
				return 0;
			LOG_ERROR("Failed to create profile: %d", ret);
			return 2;
		}

		// double-check
		PASH_PANIC_ON(_load_env());

		// re-login
		ret = ExecHost::GetInstance()
			->execl(EXEC_SERVICE, "login", "login", nullptr);
	}

	if (ret != 0)
	{
		if (ret == TERMINATION)
			return 0;
		LOG_ERROR("Failed to login: %d", ret);
		return 4;
	}
	
	ret = ExecHost::GetInstance()
		->execl(EXEC_SERVICE, "host", "host", nullptr);
	if (ret != 0)
	{
		if (ret == TERMINATION)
			return 0;
		LOG_ERROR("Error occurred in host: %d", ret);
		return 5;
	}

	if (g_env && g_doc.IsLoaded() && g_doc.IsMarked())
		g_doc.Save(g_env);

	return 0;
}

static int _ensure_env()
{
	if (!FileUtil::Exists(PASH_DIR))
	{
#if PASH_CHEAT
		FileUtil::NewDirectory(PASH_DIR, false);
#else
		FileUtil::NewDirectory(PASH_DIR, true);
#endif
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

	int size = (int)dirs.size();
	if (size != names.size())
		return 2;

	ProfilePoolPtr pool = ProfilePool::GetInstance();
	pool->Clear();
	for (int i = 0; i < size; i++)
		pool->Add(ProfilePtr(new Profile(names[i], dirs[i])));

	return 0;
}

static int _load_cache()
{
	std::string path(PASH_DIR);
	path.append(CACHE_FILE);
	FILE* fp;
	ProfilePoolPtr pool = ProfilePool::GetInstance();

	memset(_cached_user, 0, sizeof(_cached_user));

	if (fopen_s(&fp, path.c_str(), "r") != 0)
	{
		if (!pool->IsEmpty())
		{
			fopen_s(&fp, path.c_str(), "w");
			fprintf(fp, "%s\n", (*pool)[0]->username.c_str());
			fclose(fp);
		}

		if (fopen_s(&fp, path.c_str(), "r") != 0)
		{
			LOG_ERROR("Cannot create cache");
			return 1;
		}
	}

	// here, fp is open
	fscanf_s(fp, "%s", _cached_user, (unsigned int)_countof(_cached_user));
	fclose(fp);

	if (!pool->Get(_cached_user))
	{
		if (!pool->IsEmpty())
		{
			fopen_s(&fp, path.c_str(), "w");
			fprintf(fp, "%s\n", (*pool)[0]->username.c_str());
			fclose(fp);
		}
	}

	return 0;
}

static int _login_init_env()
{
	ProfilePoolPtr pool = ProfilePool::GetInstance();
	auto p = pool->Get(_cached_user);
	if (!p)
	{
		if (!pool->IsEmpty())
			p = (*pool)[0];
	}

	if (!p)
		return 1;

	g_env = CreateEnv(p);

	return 0;
}
