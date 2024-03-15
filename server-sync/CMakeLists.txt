get_filename_component(CURRENT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project ("${CURRENT_FOLDER}")

add_executable(${CURRENT_FOLDER} http_server_sync.cpp)