/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : PashDoc.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 17, 2023                          *
 *                                                                            *
 *                    Last Update : April 16, 2023                            *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Structure of password document.                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../inc/core/PashDoc.h"
#include "../../inc/core/Global.h"
#include "../../inc/core/Env.h"
#include "../../inc/common/Logger.h"
#include "../../inc/utility/PashDocUtil.h"

#include "../../inc/utility/ExecUtil.h"

#include <tea.h>
#include <algorithm>
#include <stack>


PashDoc::PashDoc() :
    m_pFile(nullptr),
    m_root(nullptr),
    m_current(nullptr),
    m_modified(false)
{
}

PashDoc::~PashDoc()
{
    UnLoad();
}

bool PashDoc::Load(EnvPtr env)
{
    if (IsLoaded())
        UnLoad();

    if (!m_pFile)
        m_pFile = std::make_unique<XMLFile>();

    FILE* input;
    const char* dataPath = env->dataPath.c_str();
    if (fopen_s(&input, dataPath, "rb") != 0)
    {
        LOG_ERROR("Missing essential file \"%s\"", dataPath);
        return false;
    }

    fseek(input, 0, SEEK_END);
    auto xml = new char[ftell(input) + 128];
    fseek(input, 0, SEEK_SET);

    char hashPass[PASSWORD_BUFFER_SIZE];
    _HashPassword(env->password, hashPass);

    auto reader = new tea::TEAFileReader(input);
    auto writer = new tea::TEABufferWriter(xml);
    decode(reader, writer, hashPass);
    delete reader;
    delete writer;

    // Now xml file is in xml buffer!
    bool ret = m_pFile->Parse(xml);
    delete[] xml;
    if (!ret)
    {
        LOG_ERROR("\t|- Perhaps wrong password or file missing?");
        LOG_ERROR("Failed to load data!");
        return false;
    }

    m_current = m_root = m_pFile->GetRoot();

    return true;
}

void PashDoc::UnLoad()
{
    if (IsLoaded())
    {
        m_pFile->UnLoad();
        m_modified = true;
    }
}


bool PashDoc::Save(EnvPtr env)
{
    if (!IsLoaded())
    {
        LOG_ERROR("Attempt to save before load!");
        return false;
    }

    XMLPrinter stream(nullptr, true, 0);
    m_pFile->Doc().Print(&stream);

    FILE* output;
    const char* dataPath = env->dataPath.c_str();
    if (fopen_s(&output, dataPath, "wb") != 0)
    {
        LOG_ERROR("Failed to open file \"%s\"", dataPath);
        return false;
    }

    char hashPass[PASSWORD_BUFFER_SIZE];
    _HashPassword(env->password, hashPass);

    auto reader = new tea::TEABufferReader(stream.CStr());
    auto writer = new tea::TEAFileWriter(output);
    encode(reader, writer, hashPass);
    delete reader;
    delete writer;

    m_modified = false;

    return true;
}

bool PashDoc::DebugLoad(EnvPtr env)
{
#ifdef PASH_CHEAT
    if (!env)
        return false;
    std::string filename(env->username);
    filename.append(".xml");

    // Protect current data.
    XMLFilePtr newFile(new XMLFile());
    if (!newFile->Load(filename.c_str()))
    {
        LOG_ERROR("Failed to import data.");
        return false;
    }

    m_pFile = std::move(newFile);
    m_root = m_pFile->GetRoot();
    m_current = m_root;

    // PashDocUtil::GetPresentWorkingDirectory(g_pwd);

    return true;
#else
	LOG_ERROR("A pathetic attempt.");
	return false;
#endif
}

bool PashDoc::DebugSave(EnvPtr env)
{
#ifdef PASH_CHEAT
    if (!env)
        return false;
    std::string filename(env->username);
    filename.append(".xml");
    m_pFile->Save(filename.c_str());
    return true;
#else
	LOG_ERROR("A pathetic attempt.");
	return false;
#endif
}

bool PashDoc::IsLoaded() const
{
    return m_pFile && (m_pFile->IsLoaded());
}

XMLElementPtr PashDoc::SetCurrent(XMLElementPtr current)
{
    if (!current)
        return m_current;

    if (!PashDocUtil::IsGroup(current))
        return nullptr;

    XMLElementPtr ret = m_current;
    m_current = current;

    return ret;
}

XMLElementPtr PashDoc::NewElement(const char* name)
{
    if (!IsLoaded())
        return nullptr;

    return m_pFile->Doc().NewElement(name);
}

void PashDoc::DeleteElement(XMLElementPtr node)
{
    m_pFile->Doc().DeleteNode(node);
}

void PashDoc::Mark()
{
    m_modified = true;
}

bool PashDoc::IsMarked() const
{
    return m_modified;
}

bool PashDoc::_GenerateData(EnvPtr env)
{
    FILE* output;
    const char* dataPath = env->dataPath.c_str();
    if (fopen_s(&output, dataPath, "wb") != 0)
    {
        LOG_ERROR("Cannot open file \"%s\"", dataPath);
        return false;
    }

    auto reader = new tea::TEABufferReader(DEFAULT_DATA);
    auto writer = new tea::TEAFileWriter(output);
    encode(reader, writer, DEFAULT_PASSWORD);
    delete reader;
    delete writer;

    return true;
}
