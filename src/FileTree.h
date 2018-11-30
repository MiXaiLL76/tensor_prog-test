#pragma once
#ifndef FILETREE_H
#define FILETREE_H

#include <string>
#include <vector>
#include <map>

class FileTree
{
  public:
    FileTree(std::string name, std::string path, std::vector<std::string> *include_path, bool local_include = true, FileTree *parent = NULL);
    std::string file_name;
    std::string file_path;

    bool local_include;
    bool exists;
    bool is_loop;
    bool print_loop;

    void print(std::string sep = "");
    std::map<std::string, int> files;
    int build();

  private:
    std::vector<std::string> *include_path;
    FileTree *parent;
    FileTree *_root;
    FileTree *getRoot();
    std::vector<FileTree> children;
};

inline char separator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

#endif