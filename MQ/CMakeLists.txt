cmake_minimum_required(VERSION 2.6)

project(MQ)

set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")

include_directories("${PROJECT_INCLUDE_DIR}" "${POCO_Foundation_INCLUDE}" "${POCO_Util_INCLUDE}" "${MQ_INCLUDE}")

add_library(mq STATIC ${SOURCES})
set_target_properties(mq PROPERTIES DEBUG_POSTFIX "d")
