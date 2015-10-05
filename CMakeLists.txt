cmake_minimum_required(VERSION 2.6)

project(MQWebSolution)

if (CMAKE_BUILD_TYPE STREQUAL debug)
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/debug)
else()
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/release)
endif()

foreach(pocolib Foundation Util XML Net JSON Data DataSQLite)

  if (CMAKE_BUILD_TYPE STREQUAL debug)
    set(pocolibname ${pocolib}d)
  else()
    set(pocolibname ${pocolib})
  endif()
  
  message(STATUS "Searching POCO library ${pocolib}")
  set(POCO_LIB POCO_${pocolib}_LIBRARY)
  
  if ( POCO_PREFIX )
    if ( POCO_NO_INSTALL )
      set(POCO_LIB_SEARCH_PATH "${POCO_PREFIX}/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}")
    else()
      set(POCO_LIB_SEARCH_PATH "${POCO_PREFIX}/lib")
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
    if ( POCO_NO_INSTALL )
      set(POCO_INC_SEARCH_PATH "${POCO_PREFIX}/../${pocolib}/include" "${POCO_PREFIX}/${pocolib}/include")
    else()
      set(POCO_INC_SEARCH_PATH "${POCO_PREFIX}/include")
    endif()
    message(STATUS "Search in ${POCO_INC_SEARCH_PATH}")
    find_path(${POCO_INCLUDE_DIR} NAMES ${POCO_INC_H}
      PATHS ${POCO_INC_SEARCH_PATH} NO_DEFAULT_PATH
    )
  endif()
  find_path(${POCO_INCLUDE_DIR} NAMES ${POCO_INC_H})

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