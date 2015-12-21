cmake_minimum_required(VERSION 2.6)

project(MQDictionary)

set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")

include_directories("${PROJECT_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../MQ/include" "${POCO_Foundation_INCLUDE}" "${POCO_Util_INCLUDE}" "${POCO_JSON_INCLUDE}" "${POCO_Net_INCLUDE}" "/opt/mqm/inc")

link_directories(${CMAKE_BINARY_DIR})

add_executable(mqcheck ${SOURCES})

if (CMAKE_BUILD_TYPE STREQUAL debug)
  set(MQLIB "mqd")
else()
  set(MQLIB "mq")
endif()

target_link_libraries(mqcheck ${MQLIB} rt dl ${POCO_Net_LIBRARY} ${POCO_Util_LIBRARY} ${POCO_JSON_LIBRARY} ${POCO_XML_LIBRARY} ${POCO_Foundation_LIBRARY})
set_target_properties(mqcheck PROPERTIES DEBUG_POSTFIX "d")
