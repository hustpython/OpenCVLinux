cmake_minimum_required(VERSION 3.6)
project(trainsvm)
set(PROJECT_VERSION "0.1")
find_package(OpenCV REQUIRED)
add_definitions(-std=c++11)
message(OpenCV_VERSION:${OpenCV_VERSION})
set(
    src 
    src/trainsvm.cpp 
)
include_directories(include)
add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
