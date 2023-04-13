/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_branch.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 13, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   View all available users.                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"

#include "../../../inc/core/Profile.h"

int _branch_list();
int _branch_create(int argc, char* argv[]);

int exec_branch(int argc, char* argv[])
{
	PASH_PANIC_ON(g_env == nullptr);

	if (argc == 1)
		return _branch_list();
	else
		return _branch_create(argc, argv);
}

int _branch_list()
{
	ProfilePool* pool = ProfilePool::GetInstance();
	Profile* profile;

	EXEC_PRINT_MSG("Available users:\n");
	for (int i = 0; i < pool->Size(); i++)
	{
		profile = &(*pool)[i];
		cnsl::InsertText("\t");
		if (profile->username == g_env->username)
			cnsl::InsertText(HIGHLIGHT_COLOR, profile->username.c_str());
		else
			cnsl::InsertText(profile->username.c_str());
		cnsl::InsertNewLine();
	}

	return 0;
}

int _branch_create(int argc, char* argv[])
{
	return ExecHost::GetInstance()->execv(EXEC_SERVICE, "profile", argv);
}