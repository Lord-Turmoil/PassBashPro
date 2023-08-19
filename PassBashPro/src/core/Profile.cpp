/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Profile.cpp                               *
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

#include "../../inc/core/Profile.h"


bool ProfilePool::Add(ProfilePtr profile)
{
    for (auto& it : m_profiles)
    {
        if (it->username == profile->username)
            return false;
    }
    m_profiles.emplace_back(profile);

    return true;
}

bool ProfilePool::Remove(const std::string& username)
{
    for (auto it = m_profiles.begin(); it != m_profiles.end(); ++it)
    {
        if ((*it)->username == username)
        {
            m_profiles.erase(it);
            return true;
        }
    }

    return false;
}

void ProfilePool::Clear()
{
    m_profiles.clear();
}

bool ProfilePool::IsEmpty()
{
    return m_profiles.empty();
}

int ProfilePool::Size()
{
    return static_cast<int>(m_profiles.size());
}

ProfilePtr ProfilePool::Get(const std::string& username)
{
    for (auto it : m_profiles)
    {
        if (it->username == username)
            return it;
    }

    return nullptr;
}
