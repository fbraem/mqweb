cmake_minimum_required(VERSION 2.6)

project(MQWeb)

set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")

include_directories("${PROJECT_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../MQ/include" "${POCO_Foundation_INCLUDE}" "${POCO_Util_INCLUDE}" "${POCO_Net_INCLUDE}" "${POCO_JSON_INCLUDE}" "${POCO_Data_INCLUDE}" "${POCO_DataSQLite_INCLUDE}" "${MQ_INCLUDE}")

if (CMAKE_BUILD_TYPE STREQUAL debug)
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/debug)
else()
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/release)
endif()

if (MQWEB_ODBC)
  add_definitions(-DMQWEB_ODBC_CONFIG)
endif()

link_directories(${CMAKE_BINARY_DIR})

add_executable(mqweb ${SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL debug)
  set(MQLIB "mqd")
else()
  set(MQLIB "mq")
endif()

if (MQWEB_ODBC)
  set(libs "${POCO_DataODBC_LIBRARY};")
  message(status ${POCO_DataODBC_LIBRARY})
endif()
list (APPEND libs ${POCO_Net_LIBRARY} ${POCO_Util_LIBRARY} ${POCO_JSON_LIBRARY} ${POCO_XML_LIBRARY} ${POCO_DataSQLite_LIBRARY} ${POCO_Data_LIBRARY} ${POCO_Foundation_LIBRARY})
message(status ${libs})

target_link_libraries(mqweb ${MQLIB} rt dl ${libs})
set_target_properties(mqweb PROPERTIES DEBUG_POSTFIX "d")
