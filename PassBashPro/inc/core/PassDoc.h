/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBash                                  *
 *                                                                            *
 *                      File Name : PassDoc.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update : April 9, 2023                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Structure of password document.                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#ifndef _PASS_DOC_H_
#define _PASS_DOC_H_

#include "../common/Constants.h"
#include "../utility/Auxiliary.h"
#include "../utility/xml.h"

#include <vector>
#include <string>


struct Env;

/********************************************************************
** Core data structure to store runtime passwords.
*/
class PassDoc
{
public:
	PassDoc();
	~PassDoc();

	bool Load(Env& env);
	void UnLoad();
	bool Save(Env& env);

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

private:
	// Generate default data.
	bool _GenerateData(Env& env);

private:
	XMLFile m_file;

	XMLElementPtr m_root;
	XMLElementPtr m_current;

	bool m_modified;
};


#endif