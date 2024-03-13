cmake_minimum_required(VERSION 3.10)

get_filename_component(CURRENT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project ("${CURRENT_FOLDER}")

add_subdirectory(server)
add_subdirectory(sync)