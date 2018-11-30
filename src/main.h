#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <string>
#include <vector>

#define SUCCESS 0
#define BASE_ERROR 1
#define INPUT_ERROR 2
#define DIR_ERROR 3

std::vector<std::string> include_path;
std::string src_path;

std::string program_name;

#endif