#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "FileTree.h"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

FileTree::FileTree(std::string name, std::string path, std::vector<std::string> *include_path, bool local_include, FileTree *parent)
    : file_name(name), file_path(path), include_path(include_path), local_include(local_include), parent(parent), exists(false)
{
    if (local_include == false)
    {
        for (int i = 0; i < include_path->size(); i++)
        {
            std::cout << (&include_path)[i] << std::endl;
            /*if (boost::filesystem::exists((&include_path)[i] + separator() + this->file_name))
            {

                break;
            }
            */
        }
        return;
    }
    this->build();
}

int FileTree::build()
{
    if (!boost::filesystem::exists(this->file_path + separator() + this->file_name))
    {
        return 1;
    }

    exists = true;

    std::ifstream file(this->file_path + separator() + this->file_name);
    std::string temp;
    bool is_comment = false;

    std::string line_comment = "//";        //
    std::string block_comment_start = "/*"; /**/
    std::string block_comment_end = "*/";   /**/
    std::string include_find = "#include";  /**/
    /*
        Это всё следует переписать на регулярные выражения. 
        Но пока не буду.
    */

    while (std::getline(file, temp))
    {

        if (temp.find(block_comment_start, 0) != std::string::npos)
        {
            if (temp.find(block_comment_end, 0) != std::string::npos)
            {
                std::string start = temp.substr(0, temp.find(block_comment_start, 0));
                std::string end = temp.substr((int)temp.find(block_comment_end) + block_comment_end.length());
                temp = start + end;
            }
            else
            {
                is_comment = true;
            }
        }

        if (is_comment && (temp.find(block_comment_end, 0) != std::string::npos))
        {
            temp = temp.substr((int)temp.find(block_comment_end) + block_comment_end.length());
            is_comment = false;
        }

        if (is_comment)
        {
            continue; //Пока мы в блоке коммента, дальше можно не выполнять
        }

        if (temp.find(line_comment, 0) != std::string::npos)
        {
            temp = temp.substr(0, (int)temp.find(line_comment, 0));
        }

        if (temp.find(include_find, 0) == std::string::npos)
        {
            continue; //в строке нет include
        }

        std::string file = temp.substr(temp.find(include_find, 0) + include_find.length());
        boost::trim(file); //обрезать пробелы на концах

        bool is_local_include = true; //тип файлов включения.

        const std::string s = file;
        const boost::regex re("[<](.*)[>]|[\"](.*)[\"]");

        boost::smatch matches;

        std::string::const_iterator pos = s.begin();
        while (boost::regex_search(pos, s.end(), matches, re))
        {
            if (!matches.empty())
            {
                if (matches[0].str()[0] == '<')
                {
                    is_local_include = false;
                }

                file = matches[0].str();
                file = file.substr(1, file.length() - 2);
                break;
            }
        }

        if (file.length() < 3) //Минимальный файл a.h, 3 символа. меньше не будет точно.
        {
            continue;
        }
        if (parent != NULL && file == parent->file_name && is_local_include == parent->local_include)
        {
            continue;
        }
        FileTree child(file, this->file_path, this->include_path, is_local_include, this);
        this->children.push_back(child);
    }
    return 0;
}

void FileTree::print(std::string sep)
{
    std::cout << sep << this->file_name << (this->exists ? "" : " (!)") << std::endl;
    sep += "..";
    for (int i = 0; i < this->children.size(); i++)
    {
        this->children[i].print(sep);
    }
}