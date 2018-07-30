cmake_minimum_required(VERSION 2.6)
PROJECT(OCRDIG)
set(PROJECT_VERSION "0.1")
find_package(OpenCV REQUIRED)
set(src src/labeldata.cpp)
ADD_EXECUTABLE( TrainData src/labeldata.cpp )
TARGET_LINK_LIBRARIES( TrainData  ${OpenCV_LIBS} )
ADD_EXECUTABLE( TestData src/trainandtest.cpp )
TARGET_LINK_LIBRARIES( TestData  ${OpenCV_LIBS} )
#TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  ${OpenCV_LIBS} )
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

SET(RESOURCES
   resources
)
foreach(f ${RESOURCES})
file( COPY ${f} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/)
endforeach(f)