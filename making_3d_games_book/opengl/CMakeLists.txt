cmake_minimum_required(VERSION 3.29.3)

# Get project name from directory
get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT_NAME} 
    VERSION 1.0.0
    DESCRIPTION "Modern C++ Game with SDL2, OpenGL"
    LANGUAGES CXX
)

# Enable compile_commands.json for LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Modern C++ standard (change to 23 for C++23 features)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wconversion -Wsign-conversion
        -Wunused -Wuninitialized
        -Wshadow -Wformat=2
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)
    else()
        add_compile_options(-O3 -DNDEBUG)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wconversion -Wsign-conversion
        -Wunused -Wuninitialized
        -Wshadow -Wformat=2
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)
    else()
        add_compile_options(-O3 -DNDEBUG)
    endif()
endif()

# Verbose makefiles (optional, disable for cleaner output)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Gather all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

# Define executable
add_executable(main ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(main
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Output directory
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Fetch SDL2 with proper configuration
include(FetchContent)

# SDL2 Main Library
FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG        release-2.32.4
    GIT_SHALLOW    TRUE
)

# SDL2_image
FetchContent_Declare(SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG        release-2.8.8
    GIT_SHALLOW    TRUE
)

# Add stb_image for OpenGL texture loading
FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb
  GIT_TAG        master
)

# RapidJSON for JSON parsing
FetchContent_Declare(rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson
  GIT_TAG        v1.1.0
  GIT_SHALLOW    TRUE
)

# Disable RapidJSON examples/tests to avoid compilation issues with strict warnings
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Configure SDL2 options before making available
set(SDL_SHARED ON CACHE BOOL "Build SDL2 shared library")
set(SDL_STATIC OFF CACHE BOOL "Build SDL2 static library")

# Make SDL2 available
FetchContent_MakeAvailable(SDL2 SDL2_image stb rapidjson)

# Create interface library for stb_image
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

# RapidJSON is header-only, add include directories
target_include_directories(main PRIVATE ${rapidjson_SOURCE_DIR}/include)


# Find system packages
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# Link libraries to target
target_link_libraries(main
PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image::SDL2_image
        GLEW::GLEW
        OpenGL::GL
        Threads::Threads
        stb_image
)

# Platform-specific configurations
if(WIN32)
    # Windows-specific settings
    target_compile_definitions(main PRIVATE WIN32_LEAN_AND_MEAN)
    if(MSVC)
        target_compile_options(main PRIVATE /W4)
    endif()
elseif(APPLE)
    # macOS-specific settings
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(main PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
elseif(UNIX)
    # Linux-specific settings
    target_link_libraries(main PRIVATE dl)
endif()

# Copy compile_commands.json to project root for LSP
if(EXISTS ${CMAKE_BINARY_DIR}/compile_commands.json)
    add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Copying compile_commands.json to project root"
    )
endif()

# Custom target for development tools
add_custom_target(dev-setup
    COMMAND ${CMAKE_COMMAND} -E echo "Setting up development environment..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Development setup complete"
)

# Testing support (optional)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    # Add test subdirectory if it exists
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        add_subdirectory(tests)
    endif()
endif()

# Install rules (optional)
install(TARGETS main
    RUNTIME DESTINATION bin
)

# Print configuration summary
message(STATUS "")
message(STATUS "Configuration Summary:")
message(STATUS "  Project Name:     ${PROJECT_NAME}")
message(STATUS "  Build Type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler:         ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Source Directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "  Binary Directory: ${CMAKE_BINARY_DIR}")
message(STATUS "")
