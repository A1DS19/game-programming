cmake_minimum_required(VERSION 3.14)

get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT_NAME} LANGUAGES CXX)

# 2. Enable compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 3. C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Verbose makefiles (optional)
set(CMAKE_VERBOSE_MAKEFILE ON)

# 5. Gather all source files (re-runs CMake if new .cpp files appear)
file(GLOB SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# 6. Define executable explicitly named “main”
add_executable(main ${SOURCES})

# 7. Include local headers (target-scoped)
target_include_directories(main
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# 8. Output directory: place binaries in <project_root>/bin
set_target_properties(main PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# 9. Fetch SDL2
include(FetchContent)
FetchContent_Declare(SDL
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG        release-2.32.4
)
FetchContent_MakeAvailable(SDL)


FetchContent_Declare(SDL_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
  GIT_TAG        release-2.8.8
)
FetchContent_MakeAvailable(SDL_image)

# 10. Link SDL2 to your “main” target
target_link_libraries(main
  PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
)

# 11. Custom target to generate .clang_complete
add_custom_target(
  generate_clang_complete
  COMMAND python ${CMAKE_SOURCE_DIR}/scripts/cc_args.py
          ${CMAKE_BINARY_DIR}/compile_commands.json
          > ${CMAKE_SOURCE_DIR}/.clang_complete
  DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
)
