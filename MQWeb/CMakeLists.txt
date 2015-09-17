cmake_minimum_required(VERSION 2.6)

project(MQWeb)

set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")

include_directories("${PROJECT_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../MQ/include" "${POCO_Foundation_INCLUDE}" "${POCO_Util_INCLUDE}" "${POCO_Net_INCLUDE}" "${POCO_JSON_INCLUDE}" "${POCO_Data_INCLUDE}" "${POCO_DataSQLite_INCLUDE}" "/opt/mqm/inc")

link_directories(${CMAKE_BINARY_DIR})

add_executable(mqweb ${SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL debug)
  target_link_libraries(mqweb debug mqd rt dl pthread ${POCO_Net_LIBRARY_DEBUG} ${POCO_Util_LIBRARY_DEBUG} ${POCO_JSON_LIBRARY_DEBUG}  ${POCO_XML_LIBRARY_DEBUG} ${POCO_DataSQLite_LIBRARY_DEBUG} ${POCO_Data_LIBRARY_DEBUG} ${POCO_Foundation_LIBRARY_DEBUG})
else()
  target_link_libraries(mqweb optimized mq rt dl pthread ${POCO_Net_LIBRARY_RELEASE} ${POCO_Util_LIBRARY_RELEASE} ${POCO_JSON_LIBRARY_RELEASE} ${POCO_XML_LIBRARY_RELEASE} ${POCO_DataSQLite_LIBRARY_RELEASE} ${POCO_Data_LIBRARY_RELEASE} ${POCO_Foundation_LIBRARY_RELEASE})
endif()

set_target_properties(mqweb PROPERTIES DEBUG_POSTFIX "d")
