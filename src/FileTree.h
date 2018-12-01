#pragma once
#ifndef FILETREE_H
#define FILETREE_H

#include <string>
#include <vector>
#include <map>

class FileTree
{
  public:
    FileTree(
		std::string name,						/* Имя файла для обработки */
		std::string path,						/* Путь, к дефолтной папке*/
		std::vector<std::string> *include_path,	/* Ссылка на папки с заголовками в формате <> */
		bool local_include = true,				/* Если true то указывает на то, что искать относительные пути.*/
		FileTree *parent = NULL					/* Родительский класс.*/
	);
    std::string file_name;						/* Название файла. возможно нужно его запихнуть в private,
												сделать доступ из функции getName(). Чтобы просто так не редактировали*/
    std::string file_path;						/* */

    bool local_include;
    bool exists;								/* Свидетельствует о существовании файла. (тоже в функцию можно)*/
    bool is_loop;								/* Имеет значение TRUE, если файл приведет к циклу.*/

    void print(std::string sep = "", std::ofstream *out_stream = NULL);			/* Рекурсивная функция вывода (рекурсия конечно не лучший выход,
												но тут это наверное самая простая реализация.*/
    std::map<std::string, int> files;			/* Список файлов. Используеться только 1 класс, т.е. ROOT*/
    int build();								/* Функция построения древа*/

  private:
    std::vector<std::string> *include_path;		/**/
    FileTree *parent;							/**/
    FileTree *_root;							/* Ссылка на самый первый элеметн в древе.*/
    FileTree *getRoot();						/* Функция для вычисления первого элеметна.
												Можно конечно и передавать ссылку всем, 
												но я захотел так.*/
    
	bool isLoop();								/* Функция проверки на циклическую зависимость*/
    std::vector<FileTree> children;				/* Список детей.*/
};

inline char separator()							/* Разделители для винды и линукса.*/
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

#endif