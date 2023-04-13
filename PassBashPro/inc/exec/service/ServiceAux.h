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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _SERVICE_AUX_H_
#define _SERVICE_AUX_H_

#include "../../core/Env.h"
#include "../../core/Profile.h"


EnvPtr CreateEnv(Profile* profile);
int DeleteProfile(Profile* profile);


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
bool VerifyProfileInit();
bool VerifyProfile(const char* password);


#endif