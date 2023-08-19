/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : PashDocUtil.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Auxiliary functions for PashDoc.                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#pragma once

#ifndef _PASS_DOC_UTIL_H_
#define _PASS_DOC_UTIL_H_

#include "../common/Constants.h"
#include "../common/Macros.h"

#include "Auxiliary.h"
#include "xml.h"

#include <string>


/********************************************************************
** Entry in Item.
*/
struct Entry
{
    const char* key;
    const char* value;
    int weight;

    Entry() : key(nullptr), value(nullptr), weight(0)
    {
    }

    Entry(const char* _k, const char* _v, int _w) :
        key(_k), value(_v), weight(_w)
    {
    }
};

struct EntryCompare
{
    bool operator()(const Entry& lhs, const Entry& rhs)
    {
        if (lhs.weight == rhs.weight)
            return strcmp(lhs.key, rhs.key) < 0;
        return lhs.weight < rhs.weight;
    };
};

using EntryList = std::vector<Entry>;


/********************************************************************
** Utility functions for PashDoc.
*/
class PashDocUtil
{
public:
    static bool IsGroup(XMLElementPtr node);
    static bool IsItem(XMLElementPtr node);

    static const char* GetNodeAttr(XMLElementPtr node, const char* attr);
    static const char* GetNodeName(XMLElementPtr node);

    static XMLElementPtr GetParentNode(XMLElementPtr node);

    static bool IsLegalNodeName(const std::string& name);


    // Group, Item, Entry
    static XMLElementPtr CreateNode(const char* tag);
    static void DeleteNode(XMLElementPtr node);

    static XMLElementPtr AddChildNode(XMLElementPtr node, XMLElementPtr child);

    static const char* GetNodeDirectory(XMLElementPtr node, std::string& path);
    static const char* GetPresentWorkingDirectory(std::string& path);


    // Group, Item
    static XMLElementPtr GetDirectChildNode(XMLElementPtr node, const char* name);
    static bool GetChildren(XMLElementPtr node, XMLElementPtrList& nodes);

    // Can return parent or self.
    static XMLElementPtr GetNodeByPath(const std::string& path);
    // Can not return parent or self.
    static XMLElementPtr GetChildNodeByPath(const std::string& path);

    static XMLElementPtr GetOrCreateChildNode(XMLElementPtr node, const char* tag, const char* name);

    static void DeleteChildNode(XMLElementPtr node, const char* name);
    static void DeleteChildren(XMLElementPtr node);

    static bool IsParent(XMLElementPtr parent, XMLElementPtr child);

    static void GetBaseName(const std::string& path, std::string& name);
    static void GetParentPath(const std::string& path, std::string& name);

    // Group
    static XMLElementPtr CreateGroupNodeByPath(const std::string& path);
    static bool GetGroupChildren(XMLElementPtr node, XMLElementPtrList& nodes);


    // Item
    static XMLElementPtr CreateItemNodeByPath(const std::string& path);

    // Entry
    static bool GetEntry(XMLElementPtr node, const char* key, Entry* entry);
    static bool GetEntries(XMLElementPtr node, EntryList& entries);

    static XMLElementPtr GetEntryNode(XMLElementPtr node, const char* key);
    static XMLElementPtr GetEntryNode(XMLElementPtr node, int id);
    static XMLElementPtr GetOrCreateEntryNode(XMLElementPtr node, const char* key);

    // If exists, it will override current one.
    bool SetEntry(XMLElementPtr node, const Entry& entry);

private:
    PashDocUtil()
    {
    }
};


struct XMLElementPtrCompare
{
    // Must be group or item! Both have name.
    bool operator()(const XMLElementPtr& lhs, const XMLElementPtr& rhs)
    {
        const char* lhsName = PashDocUtil::GetNodeName(lhs);
        const char* rhsName = PashDocUtil::GetNodeName(rhs);

        PASH_ASSERT(lhsName && rhsName);

        if (!(lhsName && rhsName))
            return false;

        bool lhsGroup = PashDocUtil::IsGroup(lhs);
        bool rhsGroup = PashDocUtil::IsGroup(rhs);

        // same type
        if ((lhsGroup && rhsGroup) || (!lhsGroup && !rhsGroup))
            return strcmp(lhsName, rhsName) < 0;

        // group in front
        return lhsGroup;
    };
};


#endif
