cmake_minimum_required(VERSION 2.6)

option(POCO_INSTALLED "Poco Installed" ON)
set(POCO_PREFIX "" CACHE STRING "Prefix Path for POCO")
if (UNIX)
  set(MQ_INSTALLATION_PATH "/opt/mqm" CACHE STRING "MQ Installation Path")
  set(MQ_INCLUDE "${MQ_INSTALLATION_PATH}/inc")
  set(MQ_LIB "${MQ_INSTALLATION_PATH}/lib64")
else()
  set(MQ_INSTALLATION_PATH "C:\\Program Files\\IBM\\WebSphere MQ" CACHE STRING "MQ Installation Path")
  set(MQ_INCLUDE "${MQ_INSTALLATION_PATH}\\Tools\\c\\include")
  set(MQ_LIB "${MQ_INSTALLATION_PATH}\\Tools\\Lib64")
endif()

project(MQWebSolution)

message(STATUS "Prefix ${POCO_PREFIX}")

if (CMAKE_BUILD_TYPE STREQUAL debug)
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/debug)
else()
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/release)
endif()

foreach(pocolib Foundation Util XML Net JSON Data DataSQLite)

  if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(pocolibname ${pocolib}d)
  else()
    set(pocolibname ${pocolib})
  endif()

  message(STATUS "Searching POCO library ${pocolib}")
  set(POCO_LIB POCO_${pocolib}_LIBRARY)

  if ( POCO_PREFIX )
    if ( POCO_INSTALLED )
      set(POCO_LIB_SEARCH_PATH "${POCO_PREFIX}/lib")
    else()
      set(POCO_LIB_SEARCH_PATH "${POCO_PREFIX}/cmake_build/lib")
    endif()
    find_library(${POCO_LIB} NAMES Poco${pocolibname}
      PATHS ${POCO_LIB_SEARCH_PATH} NO_DEFAULT_PATH
    )
  endif()
  find_library(${POCO_LIB} NAMES Poco${pocolibname})

  if (${POCO_LIB})
    message(STATUS "POCO ${pocolib} found: ${${POCO_LIB}}")
  else()
    message(FATAL_ERROR "No POCO ${pocolibname} library found!")
  endif()

  if (pocolib STREQUAL "Foundation")
    set(POCO_INC_H Poco/${pocolib}.h)
  elseif(pocolib STREQUAL "DataSQLite")
    set(POCO_INC_H Poco/Data/SQLite/SQLite.h)
  else()
    set(POCO_INC_H Poco/${pocolib}/${pocolib}.h)
  endif()
  message(STATUS "Searching ${POCO_INC_H}")

  set(POCO_INCLUDE_DIR POCO_${pocolib}_INCLUDE)
  if ( POCO_PREFIX )
    if ( POCO_INSTALLED )
      set(POCO_INC_SEARCH_PATH "${POCO_PREFIX}/include")
    else()
      if (pocolib STREQUAL "DataSQLite")
        set(POCO_INC_SEARCH_PATH "${POCO_PREFIX}/Data/SQLite/include")
      else()
        set(POCO_INC_SEARCH_PATH "${POCO_PREFIX}/${pocolib}/include")
      endif()
    endif()
    message(STATUS "Search in ${POCO_INC_SEARCH_PATH}")
    find_path(${POCO_INCLUDE_DIR} NAMES ${POCO_INC_H}
      PATHS ${POCO_INC_SEARCH_PATH} NO_DEFAULT_PATH
    )
  else()
	find_path(${POCO_INCLUDE_DIR} NAMES ${POCO_INC_H})
  endif()

  if (${POCO_INCLUDE_DIR})
    message(STATUS "POCO ${pocolib} include directory: ${${POCO_INCLUDE_DIR}}")
  else()
    message(FATAL_ERROR "No POCO ${pocolib} include directory found")
  endif()
endforeach()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

add_subdirectory(MQ)
add_subdirectory(MQWeb)
add_subdirectory(MQCheck)
add_subdirectory(MQDictionary)
