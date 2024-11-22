cmake_minimum_required(VERSION 3.28.3)

################################################################################
### PROJECT DESCRIPTION AND META INFO
################################################################################

set(EEN_PROJECT_NAME        "een")
set(EEN_PROJECT_DESCRIPTION "Embedded Edge Node")
set(EEN_AUTHOR_DOMAIN       "https://github.com/DJTakushi/een")
set(EEN_VERSION_MAJOR "0")
set(EEN_VERSION_MINOR "1")
set(EEN_VERSION_PATCH "50")

set(EEN_VERSION
    "${EEN_VERSION_MAJOR}.${EEN_VERSION_MINOR}.${EEN_VERSION_PATCH}")
set(EEN_NAME_VERSION
    "${EEN_PROJECT_NAME} v${EEN_VERSION}")

add_compile_definitions(EEN_VERSION="${EEN_NAME_VERSION}")

################################################################################
### DECLARE PROJECT
################################################################################

project(${EEN_PROJECT_NAME})

set(EEN_SRC_DIR ${PROJECT_SOURCE_DIR}/een)

add_subdirectory(app)
add_subdirectory(device_client)
add_subdirectory(een)
add_subdirectory(embedded_common)