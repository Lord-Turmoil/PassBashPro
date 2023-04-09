/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Service.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For service executables.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _SERVICE_H_
#define _SERVICE_H_

// start up
int exec_start(int argc, char* argv[]);
// register new user
int exec_register(int argc, char* argv[]);
// login
int exec_login(int argc, char* argv[]);
// logout
int exec_logout(int argc, char* argv[]);
// change master password
int exec_remaster(int argc, char* argv[]);

// receive and dispatch command
int exec_host(int argc, char* argv[]);


#endif
