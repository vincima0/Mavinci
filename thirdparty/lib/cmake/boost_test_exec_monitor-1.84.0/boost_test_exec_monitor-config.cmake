# Generated by Boost 1.84.0

if(TARGET Boost::test_exec_monitor)
  return()
endif()

if(Boost_VERBOSE OR Boost_DEBUG)
  message(STATUS "Found boost_test_exec_monitor ${boost_test_exec_monitor_VERSION} at ${boost_test_exec_monitor_DIR}")
endif()

mark_as_advanced(boost_test_exec_monitor_DIR)

# Compute the include and library directories relative to this file.

get_filename_component(_BOOST_CMAKEDIR "${CMAKE_CURRENT_LIST_DIR}/../" REALPATH)

# If the computed and the original directories are symlink-equivalent, use original
if(EXISTS "D:/boost_1_84_0/boost_1_84_0/stage/lib/cmake")
  get_filename_component(_BOOST_CMAKEDIR_ORIGINAL "D:/boost_1_84_0/boost_1_84_0/stage/lib/cmake" REALPATH)
  if(_BOOST_CMAKEDIR STREQUAL _BOOST_CMAKEDIR_ORIGINAL)
    set(_BOOST_CMAKEDIR "D:/boost_1_84_0/boost_1_84_0/stage/lib/cmake")
  endif()
  unset(_BOOST_CMAKEDIR_ORIGINAL)
endif()

get_filename_component(_BOOST_INCLUDEDIR "${_BOOST_CMAKEDIR}/../../../" ABSOLUTE)
get_filename_component(_BOOST_LIBDIR "${_BOOST_CMAKEDIR}/../" ABSOLUTE)

include(${CMAKE_CURRENT_LIST_DIR}/../BoostDetectToolset-1.84.0.cmake)

if(Boost_DEBUG)
  message(STATUS "Scanning ${CMAKE_CURRENT_LIST_DIR}/libboost_test_exec_monitor-variant*.cmake")
endif()

file(GLOB __boost_variants "${CMAKE_CURRENT_LIST_DIR}/libboost_test_exec_monitor-variant*.cmake")

macro(_BOOST_SKIPPED fname reason)
  if(Boost_VERBOSE OR Boost_DEBUG)
    message(STATUS "  [ ] ${fname}")
  endif()
  list(APPEND __boost_skipped "${fname} (${reason})")
endmacro()

list(LENGTH __boost_variants _BOOST_SINGLE_VARIANT)
if(NOT _BOOST_SINGLE_VARIANT EQUAL 1)
  set(_BOOST_SINGLE_VARIANT 0)
endif()

foreach(f IN LISTS __boost_variants)
  if(Boost_DEBUG)
    message(STATUS "  Including ${f}")
  endif()
  include(${f})
endforeach()

unset(_BOOST_SINGLE_VARIANT)
unset(_BOOST_LIBDIR)
unset(_BOOST_INCLUDEDIR)
unset(_BOOST_CMAKEDIR)

if(NOT __boost_variants AND (Boost_VERBOSE OR Boost_DEBUG))
  message(STATUS "  Library has no variants and is considered not found")
endif()

if(NOT TARGET Boost::test_exec_monitor)
  set(__boost_message "No suitable build variant has been found.")
  if(__boost_skipped)
    set(__boost_message "${__boost_message}\nThe following variants have been tried and rejected:")
    foreach(s IN LISTS __boost_skipped)
      set(__boost_message "${__boost_message}\n* ${s}")
    endforeach()
  endif()
  set(boost_test_exec_monitor_FOUND 0)
  set(boost_test_exec_monitor_NOT_FOUND_MESSAGE ${__boost_message})
  unset(__boost_message)
  unset(__boost_skipped)
  unset(__boost_variants)
  unset(_BOOST_TEST_EXEC_MONITOR_DEPS)
  return()
endif()

unset(__boost_skipped)
unset(__boost_variants)

if(_BOOST_TEST_EXEC_MONITOR_DEPS)
  list(REMOVE_DUPLICATES _BOOST_TEST_EXEC_MONITOR_DEPS)
  if(Boost_VERBOSE OR Boost_DEBUG)
    message(STATUS "Adding boost_test_exec_monitor dependencies: ${_BOOST_TEST_EXEC_MONITOR_DEPS}")
  endif()
endif()

foreach(dep_boost_test_exec_monitor IN LISTS _BOOST_TEST_EXEC_MONITOR_DEPS)
  set(_BOOST_QUIET)
  if(boost_test_exec_monitor_FIND_QUIETLY)
    set(_BOOST_QUIET QUIET)
  endif()
  set(_BOOST_REQUIRED)
  if(boost_test_exec_monitor_FIND_REQUIRED)
    set(_BOOST_REQUIRED REQUIRED)
  endif()
  get_filename_component(_BOOST_CMAKEDIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)
  find_package(boost_${dep_boost_test_exec_monitor} 1.84.0 EXACT CONFIG ${_BOOST_REQUIRED} ${_BOOST_QUIET} HINTS ${_BOOST_CMAKEDIR})
  set_property(TARGET Boost::test_exec_monitor APPEND PROPERTY INTERFACE_LINK_LIBRARIES Boost::${dep_boost_test_exec_monitor})
  unset(_BOOST_QUIET)
  unset(_BOOST_REQUIRED)
  unset(_BOOST_CMAKEDIR)
  if(NOT boost_${dep_boost_test_exec_monitor}_FOUND)
    set(boost_test_exec_monitor_FOUND 0)
    set(boost_test_exec_monitor_NOT_FOUND_MESSAGE "A required dependency, boost_${dep_boost_test_exec_monitor}, has not been found.")
    unset(_BOOST_TEST_EXEC_MONITOR_DEPS)
    return()
  endif()
endforeach()

unset(_BOOST_TEST_EXEC_MONITOR_DEPS)
