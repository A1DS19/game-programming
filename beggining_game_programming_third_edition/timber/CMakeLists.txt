cmake_minimum_required(VERSION 3.14)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2" CACHE STRING "SFML install prefix")

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system audio network)

add_library(SFML::Graphics ALIAS sfml-graphics)
add_library(SFML::Window  ALIAS sfml-window)
add_library(SFML::System  ALIAS sfml-system)
add_library(SFML::Audio   ALIAS sfml-audio)
add_library(SFML::Network ALIAS sfml-network)

add_executable(main ${SOURCES})

target_link_libraries(main PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
)

set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)


add_custom_target(
  generate_clang_complete
  COMMAND python ${CMAKE_SOURCE_DIR}/scripts/cc_args.py
          ${CMAKE_BINARY_DIR}/compile_commands.json
          > ${CMAKE_SOURCE_DIR}/.clang_complete
  DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
)
