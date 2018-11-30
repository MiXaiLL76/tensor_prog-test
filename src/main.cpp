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

    desc.add_options()("help,H", "Print help messages")("include,I", po::value<std::vector<std::string>>(), "Path to search for include files")("src,S", po::value<std::string>(), "Path to search for cpp files");

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

    std::vector<FileTree> cpp_files;
    fs::path src_dir(src_path);
    for (fs::directory_iterator it(dir), end; it != end; ++it)
    {
        if (it->path().extension() == ".cpp")
        {
            FileTree tree(it->path().filename().string(), src_path, &include_path);
            cpp_files.push_back(tree);
            tree.print();
        }
    }

    return SUCCESS;
}