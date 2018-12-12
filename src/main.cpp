#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "main.h"
#include "FileTree.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

po::options_description desc("Options"); // Опции приложения

//Функция вывода справки
void print_help()
{
    std::cout << "Use: " << program_name << " /mnt/c/mysources -I /mnt/c/mysources/includes -I /mnt/c/mylibrary" << std::endl
              << std::endl;
    std::cout << desc << std::endl;
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    /*Для вывода русских букв в консоли (тут всё в UTF-8, поэтому были каракули)*/
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    program_name = argv[0]; //	Первый аргумент всегда название приложения)

    desc.add_options()("help,H", "Вывод справки")                                            /**/
        ("include,I", po::value<std::vector<std::string>>(), "Путь к поиску include файлов") /**/
        ("src,S", po::value<std::string>(), "Путь к поиску cpp файлов")                      /**/
        ("file,F", "Вывод итоговой информации в файл.");                                     /**/

    if (argc == 1) // Если аргумент так и остался всего 1 (название приложения) то будем ругать юзера, ведь он не запускает с параметрами.
    {
        std::cout << "Input error" << std::endl;
        print_help(); // Вывод справки
        return INPUT_ERROR;
    }

    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm); // Распарсивание опций

        if (vm.count("include"))
        {
            include_path = vm["include"].as<std::vector<std::string>>(); // Загрузка include_path
        }

        if (vm.count("src"))
        {
            src_path = vm["src"].as<std::string>(); // Папка с исходными файлами
        }

        if (vm.count("help"))
        {
            print_help(); // Вывод спраки)
            return SUCCESS;
        }
        if (vm.count("file"))
        {
            print_stream.open("out.txt", std::ios::out);

            if (!print_stream)
            {
                std::cout << "Не удалось открыть файл.\nВывод будет производиться в консоли.\n";
            }
        }
        po::notify(vm);
    }
    catch (po::error &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl
                  << std::endl;
        print_help();
        return BASE_ERROR;
    }

    fs::path dir(argv[1]); // Проверяем, являеться ли 1 аргумент директорий.
    if (!fs::is_directory(dir))
    {
        dir = src_path; // Проверяем, являеться ли строка из опций директорий
        if (!fs::is_directory(dir))
        {
            std::cout << "Второй аргумент должен быть папкой с исходными файлами." << std::endl;
            std::cout << "Или используйте флаг [--src, -S] (пример -S /tmp/dir)" << std::endl;
            print_help();
            return DIR_ERROR;
        }
    }
    else
    {
        src_path.clear();         // Очистка
        src_path.append(argv[1]); // Выбор
    }

    fs::path src_dir(src_path);
    for (fs::recursive_directory_iterator it(dir), end; it != end; ++it) // Перебор файлов в папке
    {
        if (!fs::is_directory(it->path()) && it->path().extension() == ".cpp") // Поиск совпадений по расширению
        {

            /* Создания древа для 1 файла*/
            FileTree tree(it->path().filename().string(), it->path().parent_path().string(), &include_path);

            tree.build();                  // Построение древа
            tree.print("", &print_stream); // Вывод древа

            /*Чисто теоретически, можно сделать всё в 1 класс, и прикрутить многопоточность.*/
            /*Может потом займусь.*/

            total_files[it->path().filename().string()] = 0; // Добавления название *.cpp файла, как в примере с 0

            for (auto &item : tree.files) /* Обработка всех файлов, задействованных в текущем древе.*/
            {
                if (total_files.count(item.first) == 0)
                {
                    total_files[item.first] = item.second; // Создание нового элемента.
                                                           /*Можно использовать какое нибудь .insert(), но мне так нравится больше, после Golang */
                }
                else
                {
                    total_files[item.first] += item.second;
                    /*Добавление к уже существующим файлам*/
                }
            }
        }
    }
    // Разделительная строка между древом и кол-вом файлов
    if (print_stream.is_open())
    {
        print_stream << std::endl;
    }
    else
    {
        std::cout << std::endl;
    }

    std::vector<std::pair<std::string, int>> vec(total_files.begin(), total_files.end()); //Преобразование MAP в VECTOR чтобы отсортировать.
    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) { return a.second > b.second; });

    for (auto p : vec) // Вывод вектора, в соответсвии с C++11
    {
        if (print_stream.is_open())
        {
            print_stream << p.first << ' ' << p.second << std::endl;
        }
        else
        {
            std::cout << p.first << ' ' << p.second << std::endl;
        }
    }

    if (print_stream.is_open())
    {
        print_stream.close();
        std::cout << "Всё выведено в файл [out.txt].";
    }

    std::cout << std::endl
              << "Нажмите любую клавишу для продолжения...";
    std::cin.get();
    return SUCCESS;
}