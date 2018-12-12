#!/bin/bash
filename="tensor_prog-test"
version="0.8"
release="${filename}_v${version}"
tar -czpf ${release}.tar.gz CMakeLists.txt test_dir tensor_prog-test tensor_prog-test.sln src
zip -r ${release}.zip CMakeLists.txt test_dir tensor_prog-test tensor_prog-test.sln src