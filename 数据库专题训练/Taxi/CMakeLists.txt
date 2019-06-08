CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(gpt)

ADD_COMPILE_OPTIONS(-O3 -std=c++11)

LINK_LIBRARIES(metis)

ADD_EXECUTABLE(
    test
    src/GPTree.cpp
    src/GPTree.h
    src/test.cpp
)

ADD_LIBRARY(
    taxi SHARED
    src/GPTree.cpp
    src/GPTree.h
    src/taxi.cpp
)
