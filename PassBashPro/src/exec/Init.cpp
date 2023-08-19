/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Init.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Initialize all executables.                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/exec/Init.h"
#include "../../inc/exec/ExecHost.h"
#include "../../inc/exec/Exec.h"

#include "../../inc/exec/function/Function.h"
#include "../../inc/exec/editor/Editor.h"
#include "../../inc/exec/service/Service.h"

#include "../../inc/common/Macros.h"

#include "../../inc/core/Global.h"

#include "../../inc/utility/Auxiliary.h"


#define HOOK(FACTORY, DESCR, CMD) FACTORY->Hook(DESCR, CMD)

static void _InitService();
static void _InitGlobal();
static void _InitEditor();
static void _InitHidden();

void InitExecHost()
{
    _InitService();
    _InitGlobal();
    _InitEditor();
    _InitHidden();
}

void InitConsole()
{
    cnsl::InitConsoleSize(120, 30);
    cnsl::InitConsole(118);

    cnsl::SetHeader(TITLE, COPYRIGHT, AUTHOR);
    cnsl::SetTextForeground(FOREGROUND_WHITE);
    cnsl::SetTextBackground(BACKGROUND_BLACK);
    cnsl::Print();
    cnsl::OverflowReprint(false);
}

void InitAddons()
{
    SetRandomSeed();
}

static void _InitService()
{
    ExecFactoryPtr factory(new ExecFactory());

    HOOK(factory, "host", srv_host);
    HOOK(factory, "login", srv_login);
    HOOK(factory, "profile", srv_profile);
    HOOK(factory, "start", srv_start);
    HOOK(factory, "remaster", srv_remaster);
    HOOK(factory, "checkout", srv_checkout);
    HOOK(factory, "editor", exec_edit_host);
    HOOK(factory, "import", srv_import);
    HOOK(factory, "export", srv_export);
    HOOK(factory, "save", srv_save);
    HOOK(factory, "git", srv_git);

    ExecHost::GetInstance()->Register(EXEC_SERVICE, factory);
}

static void _InitGlobal()
{
    ExecFactoryPtr factory(new ExecFactory());

    HOOK(factory, "cd", exec_cd);
    HOOK(factory, "ls", exec_ls);
    HOOK(factory, "l", exec_ls);
    HOOK(factory, "touch", exec_touch);
    HOOK(factory, "mkdir", exec_mkdir);
    HOOK(factory, "rm", exec_rm);
    HOOK(factory, "move", exec_move);
    HOOK(factory, "mv", exec_move);
    HOOK(factory, "rename", exec_rename);
    HOOK(factory, "rn", exec_rename);
    HOOK(factory, "cat", exec_cat);
    HOOK(factory, "more", exec_more);
    HOOK(factory, "save", exec_save);
    HOOK(factory, "s", exec_save);
    HOOK(factory, "pwd", exec_pwd);
    HOOK(factory, "tree", exec_tree);
    HOOK(factory, "find", exec_find);
    HOOK(factory, "f", exec_find);
    HOOK(factory, "clear", exec_clear);
    HOOK(factory, "cls", exec_clear);
    HOOK(factory, "version", exec_version);
    HOOK(factory, "ver", exec_version);
    HOOK(factory, "exit", exec_exit);
    HOOK(factory, "quit", exec_exit);
    HOOK(factory, "q", exec_exit);
    HOOK(factory, "edit", exec_edit);
    HOOK(factory, "mod", exec_edit);
    HOOK(factory, "help", exec_help);
    HOOK(factory, "h", exec_help);
    HOOK(factory, "remaster", exec_remaster);
    HOOK(factory, "profile", exec_branch);
    HOOK(factory, "branch", exec_branch);
    HOOK(factory, "checkout", exec_checkout);
    HOOK(factory, "pash", exec_pash);
    HOOK(factory, "import", exec_import);
    HOOK(factory, "export", exec_export);
    HOOK(factory, "echo", exec_echo);
    HOOK(factory, "copy", exec_copy);
    HOOK(factory, "cp", exec_copy);
    HOOK(factory, "c", exec_copy);
    HOOK(factory, "flush", exec_flush);
    HOOK(factory, "fl", exec_flush);
    HOOK(factory, "git", exec_git);
    HOOK(factory, "tea", exec_tea);
    HOOK(factory, "encrypt", exec_tea);
    HOOK(factory, "generate", exec_gen);
    HOOK(factory, "gen", exec_gen);

#if PASH_CHEAT
    HOOK(factory, "order", exec_order);
#endif

    ExecHost::GetInstance()->Register(EXEC_GLOBAL, factory);
}

static void _InitEditor()
{
    ExecFactoryPtr factory(new ExecFactory());

    HOOK(factory, "clear", exec_edit_clear);
    HOOK(factory, "cls", exec_edit_clear);
    HOOK(factory, "c", exec_edit_clear);
    HOOK(factory, "help", exec_edit_help);
    HOOK(factory, "h", exec_edit_help);
    HOOK(factory, "see", exec_edit_see);
    HOOK(factory, "s", exec_edit_see);
    HOOK(factory, "more", exec_edit_more);
    HOOK(factory, "set", exec_edit_set);
    HOOK(factory, "st", exec_edit_set);
    HOOK(factory, "setk", exec_edit_setk);
    HOOK(factory, "sk", exec_edit_setk);
    HOOK(factory, "setv", exec_edit_setv);
    HOOK(factory, "sv", exec_edit_setv);
    HOOK(factory, "setw", exec_edit_setw);
    HOOK(factory, "sw", exec_edit_setw);
    HOOK(factory, "unset", exec_edit_unset);
    HOOK(factory, "ust", exec_edit_unset);
    HOOK(factory, "exit", exec_edit_exit);
    HOOK(factory, "quit", exec_edit_exit);
    HOOK(factory, "q", exec_edit_exit);

    HOOK(factory, "generate", exec_gen);
    HOOK(factory, "gen", exec_gen);

    ExecHost::GetInstance()->Register(EXEC_EDIT, factory);
}

static void _InitHidden()
{
    ExecFactoryPtr factory(new ExecFactory());

    factory = std::make_shared<ExecFactory>();
    HOOK(factory, "unk", exec_unknown);
    HOOK(factory, "edit_unk", exec_edit_unknown);
    HOOK(factory, "order", exec_order);

    ExecHost::GetInstance()->Register(EXEC_HIDDEN, factory);
}
