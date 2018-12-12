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
        std::string name,                       /* Имя файла для обработки */
        std::string path,                       /* Путь, к дефолтной папке*/
        std::vector<std::string> *include_path, /* Ссылка на папки с заголовками в формате <> */
        bool local_include = true,              /* Если true то указывает на то, что искать относительные пути.*/
        FileTree *parent = NULL                 /* Родительский класс.*/
    );
    virtual void Init(std::string name, std::string path, std::vector<std::string> *include_path, bool local_include, FileTree *parent);
    /*Я не вполне понял про виртуальный конструктор, но что то такое.*/

    virtual ~FileTree()
    {
        //std::cout << this->file_name << ": Деструктор класса." << std::endl;
        this->children.clear();
        this->files.clear();
    }

    virtual std::string get_print_sep();
    virtual void set_print_sep(std::string sep);
    virtual void print(std::string sep = "", std::ofstream *out_stream = NULL); /* Рекурсивная функция вывода (рекурсия конечно не лучший выход,
    											                                        но тут это наверное самая простая реализация.*/
    virtual std::vector<std::string> *getIncludePath();

    virtual std::vector<FileTree> *getChildrens();                                     // Получение дочерних элементов
                                                                                       //    virtual FileTree *newChild(std::string name, std::string path, bool local_include); // Создание нового дочернего элемента
    virtual FileTree newChild(std::string name, std::string path, bool local_include); // Создание нового дочернего элемента

    virtual void pushChild(FileTree *child);

    virtual int build(); /* Функция построения древа*/

    std::string file_name; /* Название файла. возможно нужно его запихнуть в private,
												сделать доступ из функции getName(). Чтобы просто так не редактировали*/
    std::string file_path; /* */

    bool local_include;
    bool exists;  /* Свидетельствует о существовании файла. (тоже в функцию можно)*/
    bool is_loop; /* Имеет значение TRUE, если файл приведет к циклу.*/

    std::map<std::string, int> files; /* Список файлов. Используеться только 1 класс, т.е. ROOT*/

  private:
    std::string default_print_sep = "  ";
    std::vector<std::string> *include_path = NULL; /**/
    FileTree *parent = NULL;                       /**/
    FileTree *_root = NULL;                        /* Ссылка на самый первый элеметн в древе.*/
    FileTree *getRoot();                           /* Функция для вычисления первого элеметна.
												Можно конечно и передавать ссылку всем, 
												но я захотел так.*/
    std::vector<FileTree> children;                /* Список детей.*/
    bool isLoop();                                 /* Функция проверки на циклическую зависимость*/
};

inline char separator() /* Разделители для винды и линукса.*/
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

#endif