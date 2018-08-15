cmake_minimum_required(VERSION 2.9)
project(nms)

find_package(OpenCV REQUIRED)
set(src 
   src/nms.cpp)
add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})