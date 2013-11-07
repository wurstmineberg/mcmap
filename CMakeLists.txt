cmake_minimum_required(VERSION 2.8)

project(mcmap C CXX)

set(mcmap_VERSION_MAJOR 0)
set(mcmap_VERSION_MINOR 1)
set(mcmap_VERSION_PATCH 2)

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
    iostreams

  REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories("/usr/local/include")

link_directories("/usr/local/lib"
                 "/usr/X11/lib")

add_subdirectory("vendor/cNBT")
add_subdirectory("vendor/json-spirit")

include_directories("vendor/cNBT/")
link_directories("${CMAKE_SOURCE_DIR}/vendor/cNBT")

include_directories("${CMAKE_SOURCE_DIR}/vendor/json-spirit/json_spirit/")

set(CMAKE_CXX_FLAGS "-Os -Wall -pedantic -std=c++11")
set(CMAKE_C_FLAGS "-Os -Wall -pedantic -std=c99")

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
  nbt
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_CONFIG_LIBRARY}
  ${Boost_FORMAT_LIBRARY}
  ${Boost_ITERATOR_LIBRARY}
  ${Boost_IOSTREAMS_LIBRARY}
  ${Boost_THREAD_LIBRARY}
)

add_dependencies(mcmap nbt json_spirit)
