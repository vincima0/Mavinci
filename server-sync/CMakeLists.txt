get_filename_component(CURRENT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project ("${CURRENT_FOLDER}")

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options("$<$<CONFIG:Debug>:/MTd>")
    add_compile_options("$<$<CONFIG:Release>:/MT>")
    add_definitions(-D_WIN32_WINNT=0x0601 -D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(${CURRENT_FOLDER} http_server_sync.cpp)