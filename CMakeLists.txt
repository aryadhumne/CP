cmake_minimum_required(VERSION 3.20)

project(OOPMultiGameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 CONFIG REQUIRED
    COMPONENTS Graphics Window System
)

add_executable(OOPMultiGameEngine main.cpp)

target_link_libraries(OOPMultiGameEngine PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)