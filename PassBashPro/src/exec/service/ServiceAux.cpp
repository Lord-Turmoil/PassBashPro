/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : ServiceAux.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 11, 2023                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   None                                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "../../../inc/exec/service/ServiceAux.h"
#include "../../../inc/exec/ExecHeader.h"
#include "../../../inc/utility/FileUtil.h"
#include "../../../inc/utility/ExecUtil.h"

#include <tea.h>
#include <hash.h>


ProfilePtr CreateProfile(const std::string& username)
{
    std::string path(PASH_DIR);
    path.append(username);

    return std::make_shared<Profile>(username, path);
}

EnvPtr CreateEnv(ProfilePtr profile)
{
    if (!profile)
        return nullptr;

    EnvPtr env(new Env());

    env->username.assign(profile->username);
    strcpy_s(env->password, DEFAULT_PASSWORD);
    env->rootPath.assign(profile->path);
    env->dataPath.assign(profile->path).append(DATA_FILE);
    env->configPath.assign(profile->path).append(CONFIG_FILE);
    env->profilePath.assign(profile->path).append(PROFILE_FILE);

    return env;
}

int DeleteProfile(ProfilePtr profile, bool force)
{
    if (!profile)
        return 0;

    // shouldn't delete current.
    if (!force && g_env && (g_env->username == profile->username))
        return 1;

    if (!FileUtil::DeletePath(profile->path.c_str()))
        return 2;

    ProfilePool::GetInstance()->Remove(profile->username);

    return 0;
}

int InitConfig(EnvPtr env)
{
    const char* configPath = env->configPath.c_str();

    if (FileUtil::Exists(configPath))
        return 0;

    if (!FileUtil::NewDirectory(env->rootPath.c_str()))
        return 1;

    FILE* output;
    if (fopen_s(&output, configPath, "wb") != 0)
    {
        LOG_ERROR("Failed to open file \"%s\"", configPath);
        return 2;
    }

    char hashPass[PASSWORD_BUFFER_SIZE];
    _HashPassword(env->password, hashPass);

    auto reader = new tea::TEARawBufferReader(hashPass, PASSWORD_MAX_LENGTH);
    auto writer = new tea::TEAFileWriter(output);
    encode(reader, writer, hashPass);
    delete reader;
    delete writer;

    return 0;
}

int InitData(EnvPtr env)
{
    const char* dataPath = env->dataPath.c_str();

    if (FileUtil::Exists(dataPath))
        return 0;

    if (!FileUtil::NewDirectory(env->rootPath.c_str()))
        return 1;

    FILE* output;
    if (fopen_s(&output, dataPath, "wb") != 0)
    {
        LOG_ERROR("Cannot open file \"%s\"", dataPath);
        return 2;
    }

    char hashPass[PASSWORD_BUFFER_SIZE];
    _HashPassword(env->password, hashPass);

    auto reader = new tea::TEABufferReader(DEFAULT_DATA);
    auto writer = new tea::TEAFileWriter(output);
    encode(reader, writer, hashPass);
    delete reader;
    delete writer;

    return 0;
}


int SaveConfig(EnvPtr env, bool overwrite)
{
    if (!FileUtil::NewDirectory(env->rootPath.c_str()))
        return 1;

    if (!overwrite && FileUtil::Exists(env->configPath.c_str()))
        return 0;

    FILE* output;
    const char* configPath = env->configPath.c_str();

    if (fopen_s(&output, configPath, "wb") != 0)
    {
        LOG_ERROR("Failed to open file \"%s\"", configPath);
        return 2;
    }

    char hashPass[PASSWORD_BUFFER_SIZE];
    _HashPassword(env->password, hashPass);

    // Prevent premature exit
    auto reader =
        new tea::TEARawBufferReader(hashPass, PASSWORD_MAX_LENGTH);
    auto writer = new tea::TEAFileWriter(output);
    encode(reader, writer, hashPass);
    delete reader;
    delete writer;

    return 0;
}

int SaveData(PashDoc& doc, EnvPtr env, bool overwrite)
{
    if (!FileUtil::NewDirectory(env->rootPath.c_str()))
        return 1;

    if (!overwrite && FileUtil::Exists(env->dataPath.c_str()))
        return 0;

    if (!doc.IsLoaded())
    {
        LOG_ERROR("Save before load");
        return 2;
    }

    doc.Save(env);

    return 0;
}

int InitEnvFiles(EnvPtr env)
{
    int ret;
    PashDoc doc;

    ret = InitConfig(env);
    if (ret != 0)
    {
        LOG_ERROR("Failed to initialize config: %d", ret);
        return 1;
    }

    ret = InitData(env);
    if (ret != 0)
    {
        LOG_ERROR("Failed to initialize data: %d", ret);
        return 2;
    }

    return 0;
}


bool UsernameVerifier(char ch)
{
    return isalnum(ch) || (ch == '_');
}

bool PasswordVerifier(char ch)
{
    return isgraph(ch);
}

bool YesNoVerifier(char ch)
{
    char lower = tolower(ch);
    return (lower == 'y') || (lower == 'n');
}

int UpdateCache()
{
    PASH_PANIC_ON(g_env == NULL);

    std::string path(PASH_DIR);
    path.append(CACHE_FILE);
    FILE* fp;

    if (fopen_s(&fp, path.c_str(), "w") != 0)
    {
        LOG_ERROR("Failed to update cache!");
        return 1;
    }
    fprintf(fp, "%s\n", g_env->username.c_str());
    fclose(fp);

    return 0;
}


static char _encoded_password[PASSWORD_BUFFER_SIZE];
static char _decoded_password[PASSWORD_BUFFER_SIZE];

bool VerifyProfileInit(EnvPtr env)
{
    FILE* input;
    const char* configPath = env->configPath.c_str();
    if (fopen_s(&input, configPath, "rb") != 0)
    {
        LOG_ERROR("Failed to open file \"%s\"", configPath);
        return false;
    }

    char buffer[32];
    auto reader = new tea::TEAFileReader(input);
    auto writer = new tea::TEABufferWriter(_encoded_password);
    reader->Read(buffer, PASSWORD_MAX_LENGTH);
    writer->Write(buffer, PASSWORD_MAX_LENGTH);
    delete reader;
    delete writer;

    return true;
}

bool VerifyProfile(const char* password)
{
    char hashPass[PASSWORD_BUFFER_SIZE];
    _HashPassword(password, hashPass);

    // MD5 vaule may contain 0x0 within!!!
    auto reader =
        new tea::TEARawBufferReader(_encoded_password, PASSWORD_MAX_LENGTH);
    auto writer = new tea::TEABufferWriter(_decoded_password);
    decode(reader, writer, hashPass);
    delete reader;
    delete writer;

    return _MEM_SAME(hashPass, _decoded_password, PASSWORD_MAX_LENGTH);
}


int VerifyData(EnvPtr env)
{
    char* data;

    FILE* input;
    if (fopen_s(&input, env->username.c_str(), "rb") != 0)
        return 1;
    fseek(input, 0, SEEK_END);
    data = new char[ftell(input) + 128];
    fseek(input, 0, SEEK_SET);

    char hashPass[PASSWORD_BUFFER_SIZE];
    _HashPassword(env->password, hashPass);

    auto reader = new tea::TEAFileReader(input);
    auto writer = new tea::TEABufferWriter(data);
    decode(reader, writer, hashPass);
    delete reader;
    delete writer;

    bool ret = XMLFile().Parse(data);
    delete[] data;

    if (!ret)
        return 2;

    return 0;
}


bool VerifyUsername(const std::string& username)
{
    if (username.length() < USERNAME_MIN_LENGTH)
    {
        EXEC_PRINT_ERR("Username too short! ");
        EXEC_PRINT_MSG("%d ~ %d characters.\n", USERNAME_MIN_LENGTH, USERNAME_MAX_LENGTH);
        return false;
    }
    if (username.length() > USERNAME_MAX_LENGTH)
    {
        EXEC_PRINT_ERR("Username too long! ");
        EXEC_PRINT_MSG("%d ~ %d characters.\n", USERNAME_MIN_LENGTH, USERNAME_MAX_LENGTH);
        return false;
    }
    for (auto c : username)
    {
        if (!UsernameVerifier(c))
        {
            EXEC_PRINT_ERR("Username contains illegal character!\n");
            EXEC_PRINT_MSG("Only [_a-zA-Z0-9] are available.\n");
            return false;
        }
    }

    return true;
}

bool VerifyPassword(const std::string& password)
{
    if (password.length() < PASSWORD_MIN_LENGTH)
    {
        EXEC_PRINT_ERR("Password too short! ");
        EXEC_PRINT_MSG("%d ~ %d characters.\n", PASSWORD_MIN_LENGTH, PASSWORD_MAX_LENGTH);
        return false;
    }
    if (password.length() > PASSWORD_MAX_LENGTH)
    {
        EXEC_PRINT_ERR("Password too long! ");
        EXEC_PRINT_MSG("%d ~ %d characters.\n", PASSWORD_MIN_LENGTH, PASSWORD_MAX_LENGTH);
        return false;
    }
    for (auto c : password)
    {
        if (!PasswordVerifier(c))
        {
            EXEC_PRINT_ERR("Password contains illegal character!\n");
            EXEC_PRINT_MSG("Only visible ASCIIs are available.\n");
            return false;
        }
    }

    return true;
}
