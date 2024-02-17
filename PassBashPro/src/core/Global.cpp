/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Global.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 12, 2023                            *
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

#include "../../inc/core/Global.h"
#include "../../inc/core/PashDoc.h"
#include "../../inc/exec/Exec.h"

#include <cnsl.h>


// Internal default password.
const char DEFAULT_PASSWORD[] = "null";

// Present working directory.
std::string g_pwd;

cnsl::InputHistory g_history;

// Current user info.
EnvPtr g_env;
bool g_isOnline;

// PashDoc
PashDoc g_doc;

// Default data content.
const char DEFAULT_DATA[] {
    R"(<?xml version="1.0" encoding="UTF-8"?>
<Group name=".">
</Group>
)"
};


// Position variables.
const int VAR_SIZE = 10;
std::vector<std::string> g_var(VAR_SIZE, "");

// Versions.
const char PASH_HOST_VERSION[] = "3.6.4";
const char PASH_EDITOR_VERSION[] = "2.1.3";

// Working mode.
int g_mode = MODE_IDLE;

// Copyright info.
const char TITLE[] { "PassBash Pro" };
const char COPYRIGHT[] { "Tony's Studio 2020 - 2024" };
const char AUTHOR[] { "Tony Skywalker" };

// Original Logo from: https://textkool.com/en/ascii-art-generator
const char LOGO[] = R"(
                  _____                    _____                    _____                    _____          
                 /\    \                  /\    \                  /\    \                  /\    \         
                /::\    \                /::\    \                /::\    \                /::\____\        
               /::::\    \              /::::\    \              /::::\    \              /:::/    /        
              /::::::\    \            /::::::\    \            /::::::\    \            /:::/    /         
             /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \          /:::/    /          
            /:::/__\:::\    \        /:::/__\:::\    \        /:::/__\:::\    \        /:::/____/           
           /::::\   \:::\    \      /::::\   \:::\    \       \:::\   \:::\    \      /::::\    \           
          /::::::\   \:::\    \    /::::::\   \:::\    \    ___\:::\   \:::\    \    /::::::\    \   _____  
         /:::/\:::\   \:::\    \  /:::/\:::\   \:::\    \  /\   \:::\   \:::\    \  /:::/\:::\    \ /\    \ 
        /:::/  \:::\   \:::\____\/:::/  \:::\   \:::\____\/::\   \:::\   \:::\____\/:::/  \:::\    /::\____\
        \::/    \:::\  /:::/    /\::/    \:::\  /:::/    /\:::\   \:::\   \::/    /\::/    \:::\  /:::/    /
         \/____/ \:::\/:::/    /  \/____/ \:::\/:::/    /  \:::\   \:::\   \/____/  \/____/ \:::\/:::/    / 
                  \::::::/    /            \::::::/    /    \:::\   \:::\    \               \::::::/    /  
                   \::::/    /              \::::/    /      \:::\   \:::\____\               \::::/    /   
                    \::/    /               /:::/    /        \:::\  /:::/    /               /:::/    /    
                     \/____/               /:::/    /          \:::\/:::/    /               /:::/    /     
                                          /:::/    /            \::::::/    /               /:::/    /      
                                         /:::/    /              \::::/    /               /:::/    /       
                                         \::/    /                \::/    /                \::/    /        
                                          \/____/                  \/____/                  \/____/         
)";

const char BANNER[] = "C O P Y R I G H T (C)  T O N Y ' S  S T U D I O  2 0 2 0 - 2 0 2 4";

const char THANKS[] = "T H A N K S  F O R  Y O U R  U S I N G";
