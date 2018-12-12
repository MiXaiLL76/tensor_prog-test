## Сборка приложения под linux с помощью CMake
##### Моя виртуальная машина для сборки:
>   Linux: ```Ubuntu 18.04 LTS```
>   gcc: ```gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0```

##### Первое что мы должны сделать, это скачать исходные файлы:

```
git clone https://github.com/MiXaiLL76/tensor_prog-test.git
cd tensor_prog-test
```

##### Создание директории:
```
mkdir build
```

##### Запуск скрипта CMake:

```
cmake ..
make -j2
```
##### Выполнение
```
./analyser ../test_dir/
```
##### Примерный вывод
```
test.cpp
  a.h
    b.h
      foo.h
    foo.h
  test.h
    foo.h
  deleted.h (!)
main.cpp
  test.h
    foo.h

foo.h 4
test.h 2
a.h 1
b.h 1
deleted.h 1
main.cpp 0
test.cpp 0

Нажмите любую клавишу для продолжения...
```
##### Дополнительно:
При появлении каких ошибок (скорее всего, недостающих библиотек) установите эти библиотеки

Минимальный boost из моего [CMakeLists.txt](./CMakeLists.txt)

```
Boost 1.63.0 COMPONENTS filesystem regex system program_options
```