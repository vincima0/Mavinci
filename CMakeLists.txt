cmake_minimum_required(VERSION 3.10)

get_filename_component(CURRENT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

include_directories(${CMAKE_SOURCE_DIR}/thirdparty)

project ("${CURRENT_FOLDER}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(server)
add_subdirectory(server-sync)
add_subdirectory(client-sync)

