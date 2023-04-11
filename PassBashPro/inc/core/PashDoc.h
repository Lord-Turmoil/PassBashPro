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
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _PASH_DOC_H_
#define _PASH_DOC_H_

#include "../common/Constants.h"
#include "../utility/Auxiliary.h"
#include "../utility/xml.h"

#include <vector>
#include <string>
#include <memory>


struct Env;

/********************************************************************
** Core data structure to store runtime passwords.
*/
class PashDoc
{
public:
	PashDoc();
	~PashDoc();

	bool Load(Env* env);
	void UnLoad();
	bool Save(Env* env);

	// For debug purpose, import and export plain text to debug.xml.
	bool DebugLoad();
	bool DebugSave();

	bool IsLoaded() const;

	XMLElementPtr GetRoot()    { return m_root; }
	XMLElementPtr GetCurrent() { return m_current; }

	/*
	** Only group can be set to current, and it returns last current.
	** If not, it returns nullptr, and do nothing.
	*/
	XMLElementPtr SetCurrent(XMLElementPtr current);

	XMLElementPtr NewElement(const char* name);
	void DeleteElement(XMLElementPtr node);

	// Mark modified.
	void Mark();

public:
	// Be careful!!!
	PashDoc& operator=(PashDoc& doc)
	{
		// Old file will be deleted automatically.
		this->m_pFile = std::move(doc.m_pFile);

		this->m_root = doc.m_root;
		this->m_current = doc.m_current;
		this->m_modified = doc.m_modified;
	};

private:
	// Generate default data.
	bool _GenerateData(Env* env);

private:
	XMLFilePtr m_pFile;

	XMLElementPtr m_root;
	XMLElementPtr m_current;

	bool m_modified;
};


#endif