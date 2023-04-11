/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : PassDoc.cpp                               *
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

#include "../../inc/core/PassDoc.h"
#include "../../inc/core/Global.h"
#include "../../inc/core/Env.h"
#include "../../inc/common/Logger.h"
#include "../../inc/utility/PashDocUtil.h"

#include <tea.h>
#include <algorithm>
#include <stack>


PassDoc::PassDoc() : m_root(nullptr), m_current(nullptr), m_modified(false) {}

PassDoc::~PassDoc() {}

bool PassDoc::Load(Env& env)
{
	if (IsLoaded())
		UnLoad();

	FILE* input;
	const char* dataPath = env.dataPath.c_str();
	if (fopen_s(&input, dataPath, "rb") != 0)
	{
		LOG_MESSAGE("Missing file \"%s\"", dataPath);
		LOG_MESSAGE("Attempt to generate default data");
		if (!_GenerateData(env))
		{
			LOG_ERROR("Failed to generate initial data");
			return false;
		}
		else if (fopen_s(&input, dataPath, "rb") != 0)
		{
			LOG_ERROR("Missing essential file \"%s\"", dataPath);
			return false;
		}
	}

	fseek(input, 0, SEEK_END);
	char* xml = new char[ftell(input) + 128];
	fseek(input, 0, SEEK_SET);

	tea::TEAFileReader* reader = new tea::TEAFileReader(input);
	tea::TEABufferWriter* writer = new tea::TEABufferWriter(xml);
	tea::decode(reader, writer, env.password);
	delete reader;
	delete writer;

	// Now xml file is in xml buffer!
	if (!m_file.Parse(xml))
	{
		LOG_ERROR("\t|- Perhaps wrong password or file missing?");
		LOG_ERROR("Failed to load data!");
		return false;
	}
	delete[] xml;

	m_current = m_root = m_file.GetRoot();

	return true;
}

void PassDoc::UnLoad()
{
	m_file.UnLoad();
}


bool PassDoc::Save(Env& env)
{
	if (!IsLoaded())
	{
		LOG_ERROR("Attempt to save before load!");
		return false;
	}

	XMLPrinter stream(0, true, 0);
	m_file.Doc().Print(&stream);

	FILE* output;
	const char* dataPath = env.dataPath.c_str();
	if (fopen_s(&output, dataPath, "wb") != 0)
	{
		LOG_ERROR("Failed to open file \"%s\"", dataPath);
		return false;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(stream.CStr());
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, env.password);
	delete reader;
	delete writer;

	m_modified = false;

	return true;
}

bool PassDoc::DebugLoad()
{
#ifdef PASH_CHEAT
	if (!m_file.Load("debug.xml"))
	{
		LOG_ERROR("Failed to import data.");
		return false;
	}

	m_root = m_file.GetRoot();
	m_current = m_root;

	// PashDocUtil::GetPresentWorkingDirectory(g_pwd);

	return true;
#else
	LOG_ERROR("A pathetic attempt.");
	return false;
#endif
}

bool PassDoc::DebugSave()
{
#ifdef PASH_CHEAT
	m_file.Save("debug.xml");
	return true;
#else
	LOG_ERROR("A pathetic attempt.");
	return false;
#endif
}

bool PassDoc::IsLoaded() const
{
	return m_file.IsLoaded();
}

XMLElementPtr PassDoc::SetCurrent(XMLElementPtr current)
{
	if (!current)
		return m_current;

	if (!PassDocUtil::IsGroup(current))
		return nullptr;

	XMLElementPtr ret = m_current;
	m_current = current;

	return ret;
}

XMLElementPtr PassDoc::NewElement(const char* name)
{
	if (!IsLoaded())
		return nullptr;

	return m_file.Doc().NewElement(name);
}

void PassDoc::DeleteElement(XMLElementPtr node)
{
	m_file.Doc().DeleteNode(node);
}

void PassDoc::Mark()
{
	m_modified = true;
}

bool PassDoc::_GenerateData(Env& env)
{
	FILE* output;
	const char* dataPath = env.dataPath.c_str();
	if (fopen_s(&output, dataPath, "wb") != 0)
	{
		LOG_ERROR("Cannot open file \"%s\"", dataPath);
		return false;
	}

	tea::TEABufferReader* reader = new tea::TEABufferReader(DEFAULT_DATA);
	tea::TEAFileWriter* writer = new tea::TEAFileWriter(output);
	tea::encode(reader, writer, g_defaultPassword);
	delete reader;
	delete writer;

	return true;
}
