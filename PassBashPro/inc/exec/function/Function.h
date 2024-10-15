/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Functions.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 19, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For all function executables.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Basic Functions
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

// cd <group name>
int exec_cd(int argc, char* argv[]);
// ls [group name]
int exec_ls(int argc, char* argv[]);
// touch <item name> [<item name> ...]
int exec_touch(int argc, char* argv[]);
// mkdir <group name> [<group name> ... ]
int exec_mkdir(int argc, char* argv[]);
// rm <node name> [<node name> ... ]
int exec_rm(int argc, char* argv[]);
// move <src> <dest>
int exec_move(int argc, char* argv[]);
// rename <old> <new>
int exec_rename(int argc, char* argv[]);
// cat <item name>
int exec_cat(int argc, char* argv[]);
// more <item name> = cat -m <item name>
int exec_more(int argc, char* argv[]);

// save
int exec_save(int argc, char* argv[]);
// pwd
int exec_pwd(int argc, char* argv[]);

// tree [group name]
int exec_tree(int argc, char* argv[]);
// find [-d] [-s] [-r root] <pattern>
int exec_find(int argc, char* argv[]);

// echo param
int exec_echo(int argc, char* argv[]);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Auxiliary Functions
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// clear
int exec_clear(int argc, char* argv[]);
// help
int exec_help(int argc, char* argv[]);
// version, ver
int exec_version(int argc, char* argv[]);
// exit
int exec_exit(int argc, char* argv[]);
// flush
int exec_flush(int argc, char* argv[]);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Advanced Functions
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// edit <item name>
int exec_edit(int argc, char* argv[]);
// tea [-d] <src> <dst> <password>
int exec_tea(int argc, char* argv[]);

// import and export encrypted data.
int exec_import(int argc, char* argv[]);
int exec_export(int argc, char* argv[]);

int exec_copy(int argc, char* argv[]);

int exec_git(int argc, char* argv[]);

// generate password
int exec_gen(int argc, char* argv[]);

// archive files to git repo
int exec_archive(int argc, char* argv[]);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Hidden Functions
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
// Handle unknown executables.
int exec_unknown(int argc, char* argv[]);
// Hand exit command.
int exec_order(int argc, char* argv[]);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Account related functions.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int exec_remaster(int argc, char* argv[]);
int exec_branch(int argc, char* argv[]);
int exec_checkout(int argc, char* argv[]);

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Easter egg.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
int exec_pash(int argc, char* argv[]);

#endif
