/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : ServiceAux.h                              *
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
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _SERVICE_AUX_H_
#define _SERVICE_AUX_H_

#include "../../core/Env.h"
#include "../../core/Profile.h"


// Create a profile from username.
ProfilePtr CreateProfile(const std::string& username);

// Create a Env from profile.
EnvPtr CreateEnv(ProfilePtr profile);

// Delete a profile physically.
int DeleteProfile(ProfilePtr profile, bool force = false);

int InitConfig(EnvPtr env);
int InitData(EnvPtr env);

/********************************************************************
** These save functions are not well designed.
*/
int SaveConfig(EnvPtr env, bool overwrite = false);
int SaveData(PashDoc& doc, EnvPtr env, bool overwrite = false);

// config
int InitEnvFiles(EnvPtr env);

bool UsernameVerifier(char ch);
bool PasswordVerifier(char ch);
bool YesNoVerifier(char ch);


// Update login cache
int UpdateCache();

// Verify profile on login.
bool VerifyProfileInit(EnvPtr env);
bool VerifyProfile(const char* password);

int VerifyData(EnvPtr env);

// Verify whole username and password.
bool VerifyUsername(const std::string& username);
bool VerifyPassword(const std::string& password);

#endif
