#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <string>
#include <vector>
#include <map>

#define SUCCESS 0						// Программа завершилась нормально
#define BASE_ERROR 1					// Программа завершилась с ошибкой опций
#define INPUT_ERROR 2					// Программа не имеет папки с исходными файлами
#define DIR_ERROR 3						// указанное, не являеться директорий

std::map<std::string, int> total_files;	//	Массив(а точнее ассоциативный контейнер) для записи всех файлов и количество включений
std::vector<std::string> include_path;	//	Пути для поиска заголовков тип <>
std::string src_path;					//	Папка с исходниками.

std::string program_name;				//	Имя программы, в принципе используеться только для сообщения помощи

std::ofstream print_stream;				// Выбор куда писать выходные данные, в файл или в консоль.
#endif