CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(quickstart)

ADD_EXECUTABLE(quickstart-client quickstart-client-main.c)
ADD_EXECUTABLE(quickstart-server quickstart-server-main.c)

INCLUDE_DIRECTORIES(. ../../)
ADD_DEFINITIONS(-DDEBUG)

