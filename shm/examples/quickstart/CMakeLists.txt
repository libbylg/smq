CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(quickstart)


#   Change the properties of files for Visual Stdio C++
IF   (MSVC)
    SET_SOURCE_FILES_PROPERTIES(quickstart-client-main.c PROPERTIES LANGUAGE CXX)
    SET_SOURCE_FILES_PROPERTIES(quickstart-server-main.c PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)



#   Where to find the libs and included files 
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../out/)


#   Use the debug mode
ADD_DEFINITIONS(-DDEBUG)


#   Define the targets
ADD_EXECUTABLE(examples-quickstart-client quickstart-client-main.c)
ADD_EXECUTABLE(examples-quickstart-server quickstart-server-main.c)


#   Which libs to link
TARGET_LINK_LIBRARIES(examples-quickstart-client smq)
TARGET_LINK_LIBRARIES(examples-quickstart-server smq)



#   Set the output dirs
SET_TARGET_PROPERTIES(examples-quickstart-client PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../out/)
SET_TARGET_PROPERTIES(examples-quickstart-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../out/)


