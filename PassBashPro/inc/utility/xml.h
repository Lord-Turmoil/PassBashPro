/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : xml.h                                     *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2022                             *
 *                                                                            *
 *                    Last Update : April 9, 2023                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The xml file class to load xml file. Since tinyxml2 does not support     *
 *   unicode, while Windows API has to use wide char, here I made a compromise*
 *   to use xml files only.                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#pragma once

#ifndef _XML_H_
#define _XML_H_

#include <vector>
#include <string>
#include <tinyxml.h>
#include <memory>

typedef std::vector<XMLElementPtr> XMLElementPtrList;

class XMLFile;
typedef std::unique_ptr<XMLFile> XMLFilePtr;


/********************************************************************
** There is a embarrassing situation that... Windows API supports
** wide char while tinyxml only support char... :(
*/
class XMLFile
{
public:
	XMLFile() : m_filename(nullptr), m_isLoaded(false) {}
	~XMLFile();

	/*
	** Parse xml file from string buffer. In this case, it won't have
	** a valid filename!
	*/
	bool Parse(const char* xml);

	/*
	** Load the file from memory. This will close the previous file.
	*/
	bool Load(const char* filename);
	bool Save();
	bool Save(const char* filename);

	/*
	** 2022/12/09 TS:
	**   Clear current document.
	*/
	bool Clear();

	/*
	** Unload the current file.
	*/
	bool UnLoad();

	/*
	** Get the root, or a specific node of the XML file.
	*/
	XMLElement* GetRoot();

	XMLElement* GetElementByTagName(const char* tag);
	XMLElement* GetElementByAttrName(const char* attr, const char* name);

	std::vector<XMLElement*> GetElementsByTagName(const char* tag);
	std::vector<XMLElement*> GetElementsByAttrName(const char* attr, const char* name);

public:
	XMLDoc& Doc() { return m_doc; }
	bool IsLoaded() const { return m_isLoaded; }

private:
	XMLDoc m_doc;
	std::string m_filename;
	bool m_isLoaded;
};


/********************************************************************
** Utility functions for XML file operations.
*/
class XMLUtil
{
public:
	static XMLElement* GetElementByTagName(XMLElement* node, const char* tag);
	static XMLElement* GetElementByAttrName(XMLElement* node, const char* attr, const char* name);
	static std::vector<XMLElement*> GetElementsByTagName(XMLElement* node, const char* tag);
	static std::vector<XMLElement*> GetElementsByAttrName(XMLElement* node, const char* attr, const char* name);

private:
	XMLUtil() {}

	static void _GetElementsByTagName(XMLElement* node, const char* tag, std::vector<XMLElement*>& results);
	static void _GetElementsByAttrName(XMLElement* node, const char* attr, const char* name, std::vector<XMLElement*>& results);
};

#endif