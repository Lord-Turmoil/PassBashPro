/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Profile.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   This is the basic structure to record user profile.                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _PROFILE_H_
#define _PROFILE_H_

#include "../template/Singleton.h"

#include <string>
#include <vector>

struct Profile
{
	std::string username;
	
	std::string path;	// root path of a user

	Profile(const std::string& _username, const std::string& _path)
		: username(_username), path(_path)
	{
	}
};

class ProfilePool : public Singleton<ProfilePool>
{
	friend class Singleton<ProfilePool>;

public:
	bool Add(const Profile& profile);
	bool Remove(const std::string& username);
	void Clear();
	bool IsEmpty();
	int Size();

	Profile* Get(const std::string& username);

	Profile& operator[](size_t id)
	{
		return m_profiles[id];
	}

private:
	std::vector<Profile> m_profiles;
};

#endif