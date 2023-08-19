/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : exec_find.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   find [-d] [-s] [path] <pattern>                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../../../inc/exec/function/FuncHeader.h"

#include <regex>


static bool is_deep;
static bool is_strict;
static std::string root_path;
static std::string pattern;
static std::regex regex_pattern;

using SearchResult = std::pair<XMLElementPtr, const char*>;
using SearchResultList = std::vector<SearchResult>;

static void _find_init();
static int _find_usage();
static int _find_parse_args(int argc, char* argv[]);
static void _search_item(XMLElementPtr root, SearchResultList& list);
static void _search(XMLElementPtr root, SearchResultList& list);
static void _find(SearchResultList& list);

int exec_find(int argc, char* argv[])
{
    _find_init();

    if (_find_parse_args(argc, argv) != 0)
    {
        _find_usage();
        return 1;
    }

    SearchResultList list;
    _find(list);

    if (list.empty())
        EXEC_PRINT_MSG("Nothing...\n");
    else
    {
        int size = static_cast<int>(list.size());
        EXEC_PRINT_MSG("Total %d %s:\n", size, (size > 1) ? "results" : "result");

        std::string path;
        int i = 0;
        for (auto it : list)
        {
            if (i < VAR_SIZE)
                cnsl::InsertText(VAR_COLOR, " $%d) ", i);
            else
                cnsl::InsertText("     ");

            cnsl::InsertText(PashDocUtil::IsGroup(it.first) ? GROUP_COLOR : ITEM_COLOR,
                             "\t%s", PashDocUtil::GetNodeDirectory(it.first, path));

            if (it.second)
                cnsl::InsertText(MESSAGE_COLOR, " [%s]\n", it.second);
            else
                cnsl::InsertNewLine();

            if (i < VAR_SIZE)
                g_var[i++] = path;
        }
    }

    return 0;
}

static void _find_init()
{
    is_deep = false;
    is_strict = false;
    PashDocUtil::GetPresentWorkingDirectory(root_path);
    pattern = "";
}

static int _find_usage()
{
    return ExecHost::GetInstance()
        ->execl(EXEC_GLOBAL, "help", "help", "find", nullptr);
}

static int _find_parse_args(int argc, char* argv[])
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while (opt = getopt(argc, argv, "ds"))
    {
        switch (opt)
        {
        case 'd':
            is_deep = true;
            break;
        case 's':
            is_strict = true;
            break;
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
                pattern = optarg;
            else if (arg_cnt == 2)
            {
                root_path = pattern;
                pattern = optarg;
            }
            else if (arg_cnt == 2)
            {
                err = true;
                EXEC_PRINT_ERR(ERRMSG_TOO_MANY "\n");
            }
            break;
        case '?':
            err = true;
            EXEC_PRINT_ERR("Unknown parameter \"-%c\"\n", optopt);
            break;
        default:
            break;
        }
    }

    if (err)
    {
        EXEC_PRINT_ERR(ERRMSG_ILLEGAL "\n");
        return 1;
    }

    return 0;
}

static void _search_item(XMLElementPtr root, SearchResultList& list)
{
    if (!PashDocUtil::IsItem(root))
        return;

    if (std::regex_match(PashDocUtil::GetNodeName(root), regex_pattern))
    {
        list.emplace_back(root, PashDocUtil::GetNodeName(root));
        return;
    }

    if (!is_deep)
        return;

    const char* key;
    const char* value;
    XMLElementPtr it = root->FirstChildElement();
    while (it)
    {
        key = PashDocUtil::GetNodeAttr(it, "key");
        if (std::regex_match(key, regex_pattern))
        {
            list.emplace_back(root, key);
            return;
        }

        // no search for sensitive entry
        if (!_IsSensitive(key))
        {
            value = PashDocUtil::GetNodeAttr(it, "value");
            if (std::regex_match(value, regex_pattern))
            {
                list.emplace_back(root, value);
                return;
            }
        }

        it = it->NextSiblingElement();
    }
}

static void _search(XMLElementPtr root, SearchResultList& list)
{
    XMLElementPtr it = root->FirstChildElement();
    while (it)
    {
        if (PashDocUtil::IsGroup(it))
        {
            if (std::regex_match(PashDocUtil::GetNodeName(it), regex_pattern))
                list.emplace_back(it, PashDocUtil::GetNodeName(it));
            _search(it, list);
        }
        else
            _search_item(it, list);

        it = it->NextSiblingElement();
    }
}

static void _find(SearchResultList& list)
{
    XMLElementPtr node = PashDocUtil::GetNodeByPath(root_path);
    if (!node || !PashDocUtil::IsGroup(node))
    {
        EXEC_PRINT_ERR("Invalid root path!\n");
        return;
    }

    if (is_strict)
        regex_pattern.assign(pattern, std::regex::icase);
    else
        regex_pattern.assign(".*" + pattern + ".*", std::regex::icase);

    _search(node, list);
}
