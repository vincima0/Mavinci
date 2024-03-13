
get_filename_component(CURRENT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project ("${CURRENT_FOLDER}")

add_executable(hello main.cpp)