cmake_minimum_required(VERSION 3.9)
project(SVM)
set(PROJECT_VERSION "0.1")
set(src src/main.cpp
        src/svm.cpp)
INCLUDE_DIRECTORIES(include)
add_executable(${PROJECT_NAME} ${src})

