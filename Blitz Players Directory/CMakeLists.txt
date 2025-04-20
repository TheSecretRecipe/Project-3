cmake_minimum_required(VERSION 3.29)
project(HashTable)

set(CMAKE_CXX_STANDARD 20)

add_executable(HashTable main.cpp HashTable.h HashTable.cpp Methods.h Methods.cpp
        BTree.h
        BTree.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

include_directories("C:/HashTable/SFML/include/SFML")
target_link_libraries(HashTable sfml-system sfml-window sfml-graphics sfml-audio)