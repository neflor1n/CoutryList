cmake_minimum_required(VERSION 3.10)
project(CountryList)

set(CMAKE_CXX_STANDARD 17)

# Подключаем библиотеки
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(main src/main.cpp)
target_link_libraries(main CURL::libcurl nlohmann_json::nlohmann_json)