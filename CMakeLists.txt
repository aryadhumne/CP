
cmake_minimum_required(VERSION 3.20)
project(MyMiniGames LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(games main.cpp)

target_link_libraries(games PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)
