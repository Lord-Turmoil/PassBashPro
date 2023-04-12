/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : xml.cpp                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2022                             *
 *                                                                            *
 *                    Last Update : April 13, 2023                            *
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

#include <string.h>

#include "../../inc/common/Error.h"
#include "../../inc/common/Logger.h"
#include "../../inc/utility/xml.h"
#include "../../inc/utility/Auxiliary.h"


/******************************************************************************
 * XMLFile::~XMLFile -- Destructor of the object.                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
XMLFile::~XMLFile()
{
	UnLoad();
}


/******************************************************************************
 * XMLFile::Parse -- Parse XML file from a string.                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   xml -- xml file buffer.                                           *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  No valid filename for this method.                              *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/01/16 Tony : Created.                                               *
 *============================================================================*/
bool XMLFile::Parse(const char* xml)
{
	int ret;

	if (m_isLoaded)
		UnLoad();

	// String does not support nullptr.
	m_filename = "";
	ret = m_doc.Parse(xml);
	if (ret != tinyxml2::XML_SUCCESS)
		return false;

	m_isLoaded = true;

	return true;
}

/******************************************************************************
 * XMLFile::Load -- Load XML file from memory.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename -- XML file name.                                        *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  Remember to unload the previous xml file.                       *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
bool XMLFile::Load(const char* filename)
{
	int ret;

	if (m_isLoaded)
		UnLoad();

	ret = m_doc.LoadFile(filename);
	if (ret != tinyxml2::XML_SUCCESS)
		return false;

	m_isLoaded = true;
	m_filename = filename;

	return true;
}

bool XMLFile::Save()
{
	if (m_filename == "")
	{
		std::string filename(GetCurrentTimestamp());
		filename.append(".xml");
		return Save(filename.c_str());
	}

	return Save(m_filename.c_str());
}

bool XMLFile::Save(const char* filename)
{
	if (!m_isLoaded)
	{
		LOG_ERROR("Attempt to save unloaded file!");
		return false;
	}

	if (_STR_SAME(filename, ""))
	{
		LOG_ERROR("Invalid filename to save!");
		return false;
	}

	int ret = m_doc.SaveFile(filename, false);
	if (ret != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR("Failed to save xml file: %d!", ret);
		return false;
	}

	return true;
}


/******************************************************************************
 * XMLFile::Clear -- Clear the xml file.                                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/12/09 Tony : Created.                                               *
 *============================================================================*/
bool XMLFile::Clear()
{
	if (!m_isLoaded)
		return false;

	m_doc.RootElement()->DeleteChildren();

	return true;
}


/******************************************************************************
 * XMLFile::UnLoad -- Unload the current xml file.                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  If no file is loaded, return false.                               *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
bool XMLFile::UnLoad()
{
	if (m_isLoaded)
	{
		m_doc.Clear();
		m_isLoaded = false;
	}

	return true;
}


/******************************************************************************
 * XMLFile::GetRoot -- Get the root of the XML file.                          *
 *                                                                            *
 *    If the file doesn't have a root, then it won't even be loaded. So there *
 *    must be a root.                                                         *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the root of the XML file. Return nullptr if no file is     *
 *          loaded.                                                           *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
XMLElement* XMLFile::GetRoot()
{
	if (!m_isLoaded)
		return nullptr;

	return m_doc.RootElement();
}


/******************************************************************************
 * XMLFile::GetElementByTagName -- Literally.                                 *
 *                                                                            *
 *    This one assumes that there's only one qualified.                       *
 *                                                                            *
 * INPUT:   node -- The parent node.                                          *
 *          tag  -- The tag name of the element.                              *
 *                                                                            *
 * OUTPUT:  Return the inquired elem. Return nullptr if the inquired elem     *
 *          doesn't exist.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
XMLElement* XMLFile::GetElementByTagName(const char* tag)
{
	return XMLUtil::GetElementByTagName(GetRoot(), tag);
}


/******************************************************************************
 * XMLFile::GetElementByAttrName -- Literally.                                *
 *                                                                            *
 *    This one assumes that there's only one qualified.                       *
 *                                                                            *
 * INPUT:   node -- The parent node.                                          *
 *          attr -- The attribute to compare.                                 *
 *          name -- The qualified attr name.                                  *
 *                                                                            *
 * OUTPUT:  Return the inquired elem. Return nullptr if the inquired elem     *
 *          doesn't exist.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
XMLElement* XMLFile::GetElementByAttrName(const char* attr, const char* name)
{
	return XMLUtil::GetElementByAttrName(GetRoot(), attr, name);
}


/******************************************************************************
 * XMLFile::GetElementsByTagName -- Literally.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   See above                                                         *
 *                                                                            *
 * OUTPUT:  Return results.                                                *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
std::vector<XMLElement*> XMLFile::GetElementsByTagName(const char* tag)
{
	return XMLUtil::GetElementsByTagName(GetRoot(), tag);
}

std::vector<XMLElement*> XMLFile::GetElementsByAttrName(const char* attr, const char* name)
{
	return XMLUtil::GetElementsByAttrName(GetRoot(), attr, name);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Utility Functions
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
XMLElement* XMLUtil::GetElementByTagName(XMLElement* node, const char* tag)
{
	if (!node)
		return nullptr;

	XMLElement* elem = node->FirstChildElement();
	XMLElement* rv;
	while (elem)
	{
		if (_STR_SAME(elem->Name(), tag))
			return elem;

		rv = GetElementByTagName(elem, tag);
		if (rv)
			return rv;

		elem = elem->NextSiblingElement();
	}

	return nullptr;
}

XMLElement* XMLUtil::GetElementByAttrName(XMLElement* node, const char* attr, const char* name)
{
	if (!node)
		return nullptr;

	XMLElement* elem = node->FirstChildElement();
	XMLElement* rv;
	while (elem)
	{
		if (elem->Attribute(attr))
		{
			if (_STR_SAME(elem->Attribute(attr), name))
				return elem;
		}

		rv = GetElementByAttrName(elem, attr, name);
		if (rv)
			return rv;

		elem = elem->NextSiblingElement();
	}

	return nullptr;
}

std::vector<XMLElement*> XMLUtil::GetElementsByTagName(XMLElement* node, const char* tag)
{
	std::vector<XMLElement*> results;

	_GetElementsByTagName(node, tag, results);

	return results;
}

std::vector<XMLElement*> XMLUtil::GetElementsByAttrName(XMLElement* node, const char* attr, const char* name)
{
	std::vector<XMLElement*> results;

	_GetElementsByAttrName(node, attr, name, results);

	return results;
}

void XMLUtil::_GetElementsByTagName(XMLElement* node, const char* tag, std::vector<XMLElement*>& results)
{
	if (!node)
		return;

	XMLElement* elem = node->FirstChildElement();
	while (elem)
	{
		if (_STR_SAME(elem->Name(), tag))
			results.push_back(elem);

		_GetElementsByTagName(elem, tag, results);

		elem = elem->NextSiblingElement();
	}
}

void XMLUtil::_GetElementsByAttrName(XMLElement* node, const char* attr, const char* name, std::vector<XMLElement*>& results)
{
	if (!node)
		return;

	XMLElement* elem = node->FirstChildElement();
	while (elem)
	{
		if (elem->Attribute(attr))
		{
			if (_STR_SAME(elem->Attribute(attr), name))
				results.push_back(elem);
		}

		_GetElementsByAttrName(elem, attr, name, results);

		elem = elem->NextSiblingElement();
	}
}
