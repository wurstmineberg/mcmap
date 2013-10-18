cmake_minimum_required(VERSION 2.8)

project(mcmap C CXX)

include(CheckCXXSourceCompiles src/)
include(CheckCSourceCompiles   src/)

include(CheckLibraryExists)

check_library_exists(z    deflate     "" HAVE_ZLIB)
find_library(png REQUIRED)

include_directories("/usr/local/include"
                    "/usr/X11/include")

link_directories("/usr/local/lib"
                 "/usr/X11/lib")

set(CMAKE_CXX_FLAGS "-Os -Wall -fomit-frame-pointer -pedantic")
set(CMAKE_CXX_LINK_FLAGS "")

aux_source_directory(src/ SOURCES)

add_executable(mcmap ${SOURCES})

target_link_libraries(mcmap png z)
