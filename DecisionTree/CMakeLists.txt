cmake_minimum_required(VERSION 2.9)
project(DECISIONTREE)
set(PROJECT_VERSION "1")
INCLUDE_DIRECTORIES(include)
find_package(OpenCV REQUIRED)
set(src 
    src/DecisionTree.cpp
    src/main.cpp)
add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

set(DATA 
   data)
foreach(f ${DATA})
file( COPY ${f} DESTINATION ./)
endforeach(i )
