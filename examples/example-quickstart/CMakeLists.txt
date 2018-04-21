CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(quickstart)


#   Create the target
ADD_EXECUTABLE(quickstart-client quickstart-client-main.c)
ADD_EXECUTABLE(quickstart-server quickstart-server-main.c)


#   Setup the properties for Visual Stdio C++
IF   (MSVC)
    SET_SOURCE_FILES_PROPERTIES(quickstart-client-main.c PROPERTIES LANGUAGE CXX)
    SET_SOURCE_FILES_PROPERTIES(quickstart-server-main.c PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)


#   Link the share librarys
TARGET_LINK_LIBRARIES(quickstart-client ${PROJECT_SOURCE_DIR}/../../smq-shared.lib)
TARGET_LINK_LIBRARIES(quickstart-server ${PROJECT_SOURCE_DIR}/../../smq-shared.lib)


#   The dir of included files 
INCLUDE_DIRECTORIES(. ../../)


#   Use the debug mode
ADD_DEFINITIONS(-DDEBUG)


#   Set the output dirs
SET_TARGET_PROPERTIES(quickstart-client PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../)
SET_TARGET_PROPERTIES(quickstart-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../)

