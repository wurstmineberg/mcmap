cmake_minimum_required(VERSION 2.8)

project(mcmap C CXX)

set(mcmap_VERSION_MAJOR 0)
set(mcmap_VERSION_MINOR 0)
set(mcmap_VERSION_PATCH 1)

include(CheckCXXSourceCompiles src/)
include(CheckCSourceCompiles   src/)

include(CheckLibraryExists)

check_library_exists(z    deflate     "" HAVE_ZLIB)
find_library(png REQUIRED)

set(Boost_REALPATH ON)
find_package(Boost 1.53
  COMPONENTS

    system
    program_options
    filesystem
    thread

  REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories("/usr/local/include")

link_directories("/usr/local/lib"
                 "/usr/X11/lib")

add_library(json_spirit STATIC IMPORTED)
set_property(TARGET json_spirit PROPERTY IMPORTED_LOCATION "/usr/local/lib/libjson_spirit.a")

set(CMAKE_CXX_FLAGS "-Os -Wall -pedantic")
set(CMAKE_CXX_LINK_FLAGS "")

aux_source_directory(src/ SOURCES)

configure_file(
  "${PROJECT_SOURCE_DIR}/src/config.hpp.in"
  "${PROJECT_SOURCE_DIR}/src/config.hpp"
)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(mcmap ${SOURCES})

target_link_libraries(mcmap 
  png 
  z 
  json_spirit
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)
