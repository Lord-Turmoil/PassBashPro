/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : PashDocUtil.cpp                           *
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
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../inc/utility/PashDocUtil.h"
#include "../../inc/core/Global.h"
#include "../../inc/core/PashDoc.h"

#include <algorithm>
#include <stack>


bool PashDocUtil::IsGroup(XMLElementPtr node)
{
    return _STR_SAME(node->Name(), GROUP_TAG);
}

bool PashDocUtil::IsItem(XMLElementPtr node)
{
    return _STR_SAME(node->Name(), ITEM_TAG);
}

const char* PashDocUtil::GetNodeAttr(XMLElementPtr node, const char* attr)
{
    return node->Attribute(attr);
}

const char* PashDocUtil::GetNodeName(XMLElementPtr node)
{
    return GetNodeAttr(node, "name");
}

XMLElementPtr PashDocUtil::GetParentNode(XMLElementPtr node)
{
    return node->Parent()->ToElement();
}

bool PashDocUtil::IsLegalNodeName(const std::string& name)
{
    return (
        (name.find('/') == std::string::npos) &&
        (name.find(' ') == std::string::npos));
}


XMLElementPtr PashDocUtil::CreateNode(const char* tag)
{
    return g_doc.NewElement(tag);
}

void PashDocUtil::DeleteNode(XMLElementPtr node)
{
    g_doc.DeleteElement(node);
}

XMLElementPtr PashDocUtil::AddChildNode(XMLElementPtr node, XMLElementPtr child)
{
    node->InsertEndChild(child);
    return child;
}

const char* PashDocUtil::GetNodeDirectory(XMLElementPtr node, std::string& path)
{
    XMLElementPtr root = g_doc.GetRoot();
    std::stack<XMLElementPtr> chain;

    while (node != root)
    {
        chain.push(node);
        node = GetParentNode(node);
    }
    chain.push(node);

    path = "";
    while (!chain.empty())
    {
        node = chain.top();
        chain.pop();
        path += GetNodeName(node);
        if (IsGroup(node))
            path += '/';
    }

    return path.c_str();
}

const char* PashDocUtil::GetPresentWorkingDirectory(std::string& path)
{
    return GetNodeDirectory(g_doc.GetCurrent(), path);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Group and Item
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElementPtr PashDocUtil::GetDirectChildNode(XMLElementPtr node, const char* name)
{
    if (!name)
        return nullptr;
    if (!IsGroup(node))
        return nullptr;

    XMLElementPtr p = node->FirstChildElement();
    while (p)
    {
        if (_STR_SAME(GetNodeName(p), name))
            return p;
        p = p->NextSiblingElement();
    }

    return nullptr;
}

bool PashDocUtil::GetChildren(XMLElementPtr node, XMLElementPtrList& nodes)
{
    nodes.clear();

    if (node->NoChildren())
        return false;

    XMLElementPtr p = node->FirstChildElement();
    while (p)
    {
        nodes.push_back(p);
        p = p->NextSiblingElement();
    }

    std::sort(nodes.begin(), nodes.end(), XMLElementPtrCompare());

    return true;
}

XMLElementPtr PashDocUtil::GetNodeByPath(const std::string& path)
{
    char* buffer = _strdup(path.c_str());
    char* context = nullptr;

    if (_STR_SAME(buffer, ROOT_DIR_NAME))
        return g_doc.GetRoot();

    // absolute or relative
    XMLElementPtr node;
    if (_STR_NSAME(buffer, ROOT_DIR_NAME))
        node = g_doc.GetRoot();
    else
        node = g_doc.GetCurrent();

    char* token = strtok_s(buffer, "/", &context);
    if (!token)
        return nullptr;

    while (node)
    {
        if (_STR_SAME(token, PARENT_DIR_NAME))
        {
            if (node != g_doc.GetRoot())
                node = GetParentNode(node);
        }
        else if (_STR_SAME(token, SELF_DIR_NAME))
            node = node;
        else
            node = GetDirectChildNode(node, token);
        token = strtok_s(nullptr, "/", &context);
        if (!token)
            break;
    }

    free(buffer); // strdup

    return node;
}

XMLElementPtr PashDocUtil::GetChildNodeByPath(const std::string& path)
{
    XMLElementPtr current = g_doc.GetCurrent();
    XMLElementPtr child = GetNodeByPath(path);

    if (IsParent(current, child))
        return child;
    return nullptr;
}

XMLElementPtr PashDocUtil::GetOrCreateChildNode(XMLElementPtr node, const char* tag, const char* name)
{
    XMLElementPtr child = GetDirectChildNode(node, name);
    if (child)
        return child;

    child = g_doc.NewElement(tag);
    child->SetAttribute("name", name);
    node->InsertEndChild(child);

    return child;
}

void PashDocUtil::DeleteChildNode(XMLElementPtr node, const char* name)
{
    XMLElementPtr child = GetDirectChildNode(node, name);
    if (child)
        DeleteNode(child);
}

void PashDocUtil::DeleteChildren(XMLElementPtr node)
{
    node->DeleteChildren();
}

bool PashDocUtil::IsParent(XMLElementPtr parent, XMLElementPtr child)
{
    XMLElementPtr root = g_doc.GetRoot();

    if (parent == child)
        return false;

    while (child != root)
    {
        child = GetParentNode(child);
        if (parent == child)
            return true;
    }

    return false;
}

void PashDocUtil::GetBaseName(const std::string& path, std::string& name)
{
    size_t pos = path.find_last_of('/');

    if (pos == std::string::npos)
        name = path;
    else
    {
        if (path.back() != '/') // is item
            name = path.substr(pos + 1);
        else
        {
            std::string temp = path;
            while (!temp.empty() && (temp.back() == '/'))
                temp.pop_back();
            pos = temp.find_last_of('/');
            if (pos == std::string::npos)
                name = temp;
            else
                name = temp.substr(pos + 1);
        }
    }
}

void PashDocUtil::GetParentPath(const std::string& path, std::string& name)
{
    std::string temp = path;
    while (!temp.empty() && temp.back() == '/')
        temp.pop_back();
    size_t pos = temp.find_last_of('/');
    if (pos == std::string::npos)
        name = "";
    else
        name = temp.substr(0, pos);

    // root directory
    if (name == ".")
        name.append("/");
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Group
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElementPtr PashDocUtil::CreateGroupNodeByPath(const std::string& path)
{
    char* buffer = _strdup(path.c_str());
    char* context = nullptr;

    if (_STR_SAME(buffer, ROOT_DIR_NAME))
        return g_doc.GetRoot();

    XMLElementPtr node;
    if (_STR_NSAME(buffer, ROOT_DIR_NAME))
        node = g_doc.GetRoot();
    else
        node = g_doc.GetCurrent();

    char* token = strtok_s(buffer, "/", &context);
    while (token)
    {
        if (_STR_SAME(token, PARENT_DIR_NAME))
        {
            if (node != g_doc.GetRoot())
                node = GetParentNode(node);
        }
        else if (_STR_SAME(token, SELF_DIR_NAME))
            node = node;
        else
            node = GetOrCreateChildNode(node, GROUP_TAG, token);

        token = strtok_s(nullptr, "/", &context);
    }

    free(buffer);

    return node;
}

bool PashDocUtil::GetGroupChildren(XMLElementPtr node, XMLElementPtrList& nodes)
{
    nodes.clear();

    XMLElementPtr p = node->FirstChildElement();
    while (p)
    {
        if (IsGroup(p))
            nodes.push_back(p);
        p = p->NextSiblingElement();
    }

    std::sort(nodes.begin(), nodes.end(), XMLElementPtrCompare());

    return true;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Item
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElementPtr PashDocUtil::CreateItemNodeByPath(const std::string& path)
{
    std::string name;

    GetBaseName(path, name);
    if (name == "") // illegal name.
        return nullptr;

    char* buffer = _strdup(path.c_str());
    char* context = nullptr;

    if (_STR_SAME(buffer, ROOT_DIR_NAME))
        return g_doc.GetRoot();

    // Absolute or relative.
    XMLElementPtr node;
    if (_STR_NSAME(buffer, ROOT_DIR_NAME))
        node = g_doc.GetRoot();
    else
        node = g_doc.GetCurrent();

    char* token = strtok_s(buffer, "/", &context);
    while (token)
    {
        if (_STR_SAME(token, PARENT_DIR_NAME))
        {
            if (node != g_doc.GetRoot())
                node = GetParentNode(node);
        }
        else if (_STR_SAME(token, SELF_DIR_NAME))
            node = node;
        else
            node = GetOrCreateChildNode(node, GROUP_TAG, token);

        token = strtok_s(nullptr, "/", &context);
    }

    free(buffer); // strdup

    if (node)
    {
        node->SetName(ITEM_TAG);
        node->SetAttribute("name", name.c_str());
    }

    return node;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Entry
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool PashDocUtil::GetEntry(XMLElementPtr node, const char* key, Entry* entry)
{
    if (!IsItem(node))
        return false;

    XMLElementPtr p = node->FirstChildElement();
    while (p)
    {
        if (_STR_SAME(GetNodeAttr(p, "key"), key))
            break;
        p = p->NextSiblingElement();
    }

    if (p)
    {
        if (entry)
        {
            entry->key = key;
            entry->value = p->Attribute("value");
            entry->weight = p->IntAttribute("weight");
        }
        return true;
    }

    return false;
}

bool PashDocUtil::GetEntries(XMLElementPtr node, EntryList& entries)
{
    if (!IsItem(node))
        return false;

    entries.clear();

    if (node->NoChildren())
        return true;

    XMLElementPtr p = node->FirstChildElement();
    while (p)
    {
        entries.emplace_back(
            p->Attribute("key"),
            p->Attribute("value"),
            p->IntAttribute("weight"));

        p = p->NextSiblingElement();
    }

    std::sort(entries.begin(), entries.end(), EntryCompare());

    return true;
}

XMLElementPtr PashDocUtil::GetEntryNode(XMLElementPtr node, const char* key)
{
    if (!IsItem(node))
        return nullptr;

    XMLElementPtr p = node->FirstChildElement();
    while (p)
    {
        if (_STR_SAME(GetNodeAttr(p, "key"), key))
            break;
        p = p->NextSiblingElement();
    }

    return p;
}

XMLElementPtr PashDocUtil::GetEntryNode(XMLElementPtr node, int id)
{
    if (!IsItem(node))
        return nullptr;
    if (id < 0)
        return nullptr;

    EntryList list;
    GetEntries(node, list);
    if (id < list.size())
        return GetEntryNode(node, list[id].key);
    return nullptr;
}

XMLElementPtr PashDocUtil::GetOrCreateEntryNode(XMLElementPtr node, const char* key)
{
    if (!IsItem(node))
        return nullptr;

    XMLElementPtr p = GetEntryNode(node, key);
    if (!p)
    {
        p = CreateNode(ENTRY_TAG);
        p->SetAttribute("key", key);
        AddChildNode(node, p);
    }

    return p;
}

// If exists, it will override current one.
bool PashDocUtil::SetEntry(XMLElementPtr node, const Entry& entry)
{
    XMLElementPtr child = GetOrCreateEntryNode(node, entry.key);
    if (!child)
        return false;

    child->SetAttribute("value", entry.value);
    child->SetAttribute("weight", entry.weight);

    return true;
}
