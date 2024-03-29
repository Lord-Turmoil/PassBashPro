/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Service.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 19, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For service executables.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _SERVICE_H_
#define _SERVICE_H_

// start up
int srv_start(int argc, char* argv[]);

// create a new local profile
int srv_profile(int argc, char* argv[]);

// switch user
int srv_checkout(int argc, char* argv[]);

// login
int srv_login(int argc, char* argv[]);

// change master password
int srv_remaster(int argc, char* argv[]);

// receive and dispatch command
int srv_host(int argc, char* argv[]);

int srv_save(int argc, char* argv[]);

int srv_import(int argc, char* argv[]);
int srv_export(int argc, char* argv[]);

int srv_git(int argc, char* argv[]);

int srv_archive(int argc, char* argv[]);

#endif
