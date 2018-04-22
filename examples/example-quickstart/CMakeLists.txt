CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(quickstart)


#   Create the target
ADD_EXECUTABLE(quickstart-client quickstart-client-main.c)
ADD_EXECUTABLE(quickstart-server quickstart-server-main.c)


#   Change the properties of files for Visual Stdio C++
IF   (MSVC)
    SET_SOURCE_FILES_PROPERTIES(quickstart-client-main.c PROPERTIES LANGUAGE CXX)
    SET_SOURCE_FILES_PROPERTIES(quickstart-server-main.c PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)


#   Link the share librarys
#LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../out/)
SET(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/../../out/)
TARGET_LINK_LIBRARIES(quickstart-client smq)
TARGET_LINK_LIBRARIES(quickstart-server smq)


#   The dir of included files 
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../)


#   Use the debug mode
ADD_DEFINITIONS(-DDEBUG)


#   Set the output dirs
SET_TARGET_PROPERTIES(quickstart-client PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../out/)
SET_TARGET_PROPERTIES(quickstart-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../out/)

