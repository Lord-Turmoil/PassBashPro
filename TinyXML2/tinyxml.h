/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : tinyxml.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 7, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   A intermediatry for tinyxml2.                                            *
 * -------------------------------------------------------------------------- *
 * REFERENCE:                                                                 *
 *   https://github.com/leethomason/tinyxml2                                  *
 *   https://www.cnblogs.com/happykoukou/p/6307257.html                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 *   Windows 11 Pro                                                           *
 ******************************************************************************/

#ifndef _TINYXML_H_
#define _TINYXML_H_

#include "tinyxml2.h"

// There's a conflict with XMLDocument in msxml.h. :(
using XMLDoc = tinyxml2::XMLDocument;

using tinyxml2::XMLElement;
using XMLElementPtr = XMLElement*;
using tinyxml2::XMLNode;
using XMLNodePtr = XMLNode*;

using tinyxml2::XMLPrinter;


#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "tinyxml2_b.lib")
#else
#pragma comment(lib, "tinyxml2.lib")
#endif

#endif
