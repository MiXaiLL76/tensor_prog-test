#pragma once
#ifndef FILETREE_H
#define FILETREE_H

#include <string>
#include <vector>

class FileTree
{
  public:
    FileTree(std::string name, std::string path, std::vector<std::string> *include_path, bool local_include = true, FileTree *parent = NULL);
    std::string file_name;
    std::string file_path;
    bool local_include;
    bool exists;
    std::vector<FileTree> children;
    void print(std::string sep = "");

  private:
    std::vector<std::string> *include_path;
    FileTree *parent;
    int build();
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