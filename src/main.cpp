#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "main.h"
#include "FileTree.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

po::options_description desc("Options");

void print_help()
{
    std::cout << "Use: " << program_name << " /mnt/c/mysources -I /mnt/c/mysources/includes -I /mnt/c/mylibrary" << std::endl
              << std::endl;
    std::cout << desc << std::endl;
}

int main(int argc, char **argv)
{
    program_name = argv[0];

    desc.add_options()("help,H", "Вывод справки")("include,I", po::value<std::vector<std::string>>(), "Путь к поиску include файлов")("src,S", po::value<std::string>(), "Путь к поиску cpp файлов")("loop", "Вывод информации о циклически включенных файлах.");

    if (argc == 1)
    {
        std::cout << "Input error" << std::endl;
        print_help();
        return INPUT_ERROR;
    }
    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("include"))
        {
            include_path = vm["include"].as<std::vector<std::string>>();
        }

        if (vm.count("src"))
        {
            src_path = vm["src"].as<std::string>();
        }
        if (vm.count("loop"))
        {
            print_loop = true;
        }

        if (vm.count("help"))
        {
            print_help();
            return SUCCESS;
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

    fs::path dir(argv[1]);
    if (!fs::is_directory(dir))
    {
        dir = src_path;
        if (!fs::is_directory(dir))
        {
            std::cout << "The second argument must be a folder with source files." << std::endl;
            std::cout << "Or use [--src, -S] flag to select directory" << std::endl;
            print_help();
            return DIR_ERROR;
        }
    }
    else
    {
        src_path.clear();
        src_path.append(argv[1]);
    }

    std::map<std::string, int> total_files;

    fs::path src_dir(src_path);
    for (fs::directory_iterator it(dir), end; it != end; ++it)
    {
        if (it->path().extension() == ".cpp")
        {
            FileTree tree(it->path().filename().string(), src_path, &include_path);
            tree.print_loop = print_loop;
            tree.build();
            tree.print();

            total_files[it->path().filename().string()] = 0;

            for (auto &item : tree.files)
            {
                if (total_files.count(item.first) == 0)
                {
                    total_files[item.first] = item.second;
                }
                else
                {
                    total_files[item.first] += item.second;
                }
            }
        }
    }
    std::cout << std::endl;

    std::vector<std::pair<std::string, int>> vec(total_files.begin(), total_files.end());
    std::sort(vec.begin(), vec.end(), [] (const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) { return a.second > b.second; });
    for (auto p : vec)
        std::cout << p.first << ' ' << p.second << std::endl;

    return SUCCESS;
}