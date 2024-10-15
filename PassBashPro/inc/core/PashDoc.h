/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : PashDoc.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update : April 11, 2023                            *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Structure of password document.                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#pragma once

#ifndef _PASH_DOC_H_
#define _PASH_DOC_H_

#include "../utility/xml.h"

struct Env;
using EnvPtr = std::shared_ptr<Env>;

/********************************************************************
** Core data structure to store runtime passwords.
*/
class PashDoc
{
public:
    PashDoc();
    ~PashDoc();

    bool Load(EnvPtr env);
    void UnLoad();
    bool Save(EnvPtr env);

    // For debug purpose, import and export plain text to debug.xml.
    bool DebugLoad(EnvPtr env);
    bool DebugSave(EnvPtr env);

    bool IsLoaded() const;

    XMLElementPtr GetRoot()
    {
        return m_root;
    }

    XMLElementPtr GetCurrent()
    {
        return m_current;
    }

    /*
    ** Only group can be set to current, and it returns last current.
    ** If not, it returns nullptr, and do nothing.
    */
    XMLElementPtr SetCurrent(XMLElementPtr current);

    XMLElementPtr NewElement(const char* name);
    void DeleteElement(XMLElementPtr node);

    // Mark modified.
    void Mark();
    bool IsMarked() const;

public:
    // Be careful!!!
    PashDoc& operator=(PashDoc& doc)
    {
        // Old file will be deleted automatically.
        this->m_pFile = std::move(doc.m_pFile);

        this->m_root = doc.m_root;
        this->m_current = doc.m_current;
        this->m_modified = doc.m_modified;

        return *this;
    };

private:
    // Generate default data.
    bool _GenerateData(EnvPtr env);

private:
    XMLFilePtr m_pFile;

    XMLElementPtr m_root;
    XMLElementPtr m_current;

    bool m_modified;
};

#endif
