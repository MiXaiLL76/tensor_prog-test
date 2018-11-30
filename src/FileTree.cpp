#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "FileTree.h"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

/* Инициализация класса.*/
FileTree::FileTree(std::string name, std::string path, std::vector<std::string> *include_path, bool local_include, FileTree *parent)
    : file_name(name), file_path(path), include_path(include_path), local_include(local_include), parent(parent), exists(false), is_loop(false)
{
	if (local_include == false)
    {
        for (size_t i = 0; i < include_path->size(); i++)
        {
            if (boost::filesystem::exists((*include_path)[i] + separator() + this->file_path+ separator() +this->file_name))
            {
                this->file_path = (*include_path)[i];
                break;
            }
        }
    }
}

int FileTree::build()
{
    if (!boost::filesystem::exists(this->file_path + separator() + this->file_name))
    {
        return 1;		/* Если файла не существует, читать его нельзя*/
    }

    exists = true;		/* Файл найден.*/

	/*
	Читать файл буду построчно, по идее, так меньше места в памяти будет занимать, но выполнение будет дольше. 
	Палка о двух концах)
	*/
    std::ifstream file(this->file_path + separator() + this->file_name);
    std::string temp;	// Переменная для хранения строки
    bool is_comment = false;

    std::string line_comment = "//";        //	Коментарий формата "//" всё, что после него, удаляем.
    std::string block_comment_start = "/*"; // Начало блока коммента /* 
    std::string block_comment_end = "*/";   // Конец блока коммента  */
    std::string include_find = "#include";  // Так выглядит то, что мы должны обработать
    
	/*
        Это всё следует переписать на регулярные выражения. 
        Но пока не буду.
    */

    while (std::getline(file, temp))		// Построчное чтение
    {
        if (temp.find(block_comment_start, 0) != std::string::npos)// Если начался блок коментариев
        {
            if (temp.find(block_comment_end, 0) != std::string::npos)//Если он закончился в этой же строке.
            {
                std::string start = temp.substr(0, temp.find(block_comment_start, 0));
                std::string end = temp.substr((int)temp.find(block_comment_end) + block_comment_end.length());
                temp = start + end;		// В темп пишем всё, что между этими двумя строками
            }
            else
            {
                is_comment = true;		// Если комментарий только начался в этой строке, а закончится после.
            }
        }

        if (is_comment && (temp.find(block_comment_end, 0) != std::string::npos))// Если нашли конец незакрытого блока комметария.
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
			// Уничтожаем все данные, после линейного комментария.
        }

        if (temp.find(include_find, 0) == std::string::npos)
        {
            continue; //в строке нет include
        }

        std::string file = temp.substr(temp.find(include_find, 0) + include_find.length());
        boost::trim(file); //обрезать пробелы на концах

        bool is_local_include = true; //тип файлов включения.

        const std::string s = file;
        const boost::regex re("[<](.*)[>]|[\"](.*)[\"]");// Выбираем по этому регулярному выражению всё, что заключено в скобки формата <> и ""

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
		
		//Строим древо для этого дочернего элемента.
		//boost::replace_all(file, "/", separator());
		std::replace_copy(file.begin(), file.end(), file.begin(), '/', separator());
		boost::filesystem::path target(file);

        FileTree child(target.filename().string(), file_path+ separator()+target.parent_path().string(), this->include_path, is_local_include, this);
        if(child.isLoop()){
            child.exists = true;
        }else{
			this->getRoot();
            child.build();
        }

		/*Подсчёт файликов*/
        if (this->_root->files.count(target.filename().string()) == 0){
				 this->_root->files[target.filename().string()] = 0;
		}       
		this->_root->files[target.filename().string()]++;

        this->children.push_back(child);
        
    }
    return 0;
}

/*Проход по элеметнам*/
FileTree *FileTree::getRoot()
{
	FileTree *root = this;
    while (root->parent != NULL)
    {
        root = root->parent;
    }
	this->_root = root;
    return root;
}

bool FileTree::isLoop(){
    FileTree *root = this;
    while (root->parent != NULL)
    {
        if(
			root->parent->file_name == this->file_name			/* Имя совпадает*/
			&&
			root->parent->local_include == this->local_include	/* Тип включения совпадает*/
		){
            this->is_loop = true;
            return true;
        }
        root = root->parent;        
    }
    return false;
}

void FileTree::print(std::string sep)
{
    std::cout << sep << this->file_name << (this->exists ? "" : " (!)")/* << (this->is_loop ? " (LOOP)" : "") */<< std::endl;
    sep += "  ";
    for (size_t i = 0; i < this->children.size(); i++)
    {
        this->children[i].print(sep);
    }
}