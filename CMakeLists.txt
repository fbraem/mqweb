cmake_minimum_required(VERSION 2.6)

project(MQWebSolution)

if (CMAKE_BUILD_TYPE STREQUAL debug)
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/debug)
else()
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/release)
endif()

set(POCO_INCLUDE_DIRS)
foreach(pocolib Foundation Util XML Net JSON Data DataSQLite)
  message(STATUS "POCO ${pocolib} - RELEASE")
  set(POCO_LIB_RELEASE POCO_${pocolib}_LIBRARY_RELEASE)
  find_library(${POCO_LIB_RELEASE} NAMES Poco${pocolib}
    PATHS ${POCO_PREFIX}/static/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}
          /usr/lib
          /usr/local/lib
          /usr/lib64
          /usr/local/lib64
  )
  if (${POCO_LIB_RELEASE})
    message(STATUS "POCO ${pocolib} found: ${${POCO_LIB_RELEASE}}")
  else()
    message(FATAL_ERROR "No POCO ${pocolib} library found!")
  endif()
  
  message(STATUS "POCO ${pocolib}d - DEBUG")
  set(POCO_LIB_DEBUG POCO_${pocolib}_LIBRARY_DEBUG)
  find_library(${POCO_LIB_DEBUG} NAMES Poco${pocolib}d
    PATHS ${POCO_PREFIX}/static/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}
          /usr/lib
          /usr/local/lib
          /usr/lib64
          /usr/local/lib64
  )
  if (${POCO_LIB_DEBUG})
    message(STATUS "POCO ${pocolib}d found: ${${POCO_LIB_DEBUG}}")
  else()
    message(STATUS "No POCO ${pocolib}d DEBUG library found! Debug build will be skipped")
  endif()
  
  message(STATUS "Searching ${pocolib}.h")
  if (pocolib STREQUAL "Foundation")
    find_path(POCO_${pocolib}_INCLUDE NAMES Poco/${pocolib}.h
      PATHS ${POCO_PREFIX}/${pocolib}/include
      /usr/include
      /usr/local/include
    )
  elseif(pocolib STREQUAL "DataSQLite")
    find_path(POCO_${pocolib}_INCLUDE NAMES Poco/Data/SQLite/SQLite.h
      PATHS ${POCO_PREFIX}/Data/SQLite/include
      /usr/include/Poco
      /usr/local/include/Poco
    )
  else()
    find_path(POCO_${pocolib}_INCLUDE NAMES Poco/${pocolib}/${pocolib}.h
      PATHS ${POCO_PREFIX}/${pocolib}/include
      /usr/include/Poco
      /usr/local/include/Poco
    )
  endif()
  set(POCO_INCLUDE_DIR POCO_${pocolib}_INCLUDE)
  if (POCO_INCLUDE_DIR)
    message(STATUS "POCO ${pocolib} include directory: ${${POCO_INCLUDE_DIR}}")
  else()
    message(FATAL_ERROR "No POCO ${pocolib} include directory found")
  endif()
endforeach()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(MQ)
add_subdirectory(MQWeb)
