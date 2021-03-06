cmake_minimum_required(VERSION 2.8)

project(mcmap C CXX)

set(mcmap_VERSION_MAJOR 0)
set(mcmap_VERSION_MINOR 1)
set(mcmap_VERSION_PATCH 2)

exec_program(
  "git"
  ${CMAKE_CURRENT_SOURCE_DIR}
  ARGS "log -n1 --format=\"%h\""
  OUTPUT_VARIABLE mcmap_VERSION_HASH 
)

set(mcmap_BASEPATH "${CMAKE_INSTALL_PREFIX}/share/mcmap")

if (${UNIX}) 
  add_definitions(-D__UNIX__) 
endif()

if (${APPLE}) 
  add_definitions(-D__APPLE__) 
endif()

include(CheckCXXSourceCompiles src/)
include(CheckCSourceCompiles   src/)

include(CheckLibraryExists)

# clean all command, see 
# http://stackoverflow.com/questions/9680420/looking-for-a-cmake-clean-command-to-clear-up-cmake-output
add_custom_target(clean-cmake-files
   COMMAND ${CMAKE_COMMAND} -P clean-all.cmake
)

add_custom_target(clean-all
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND ${CMAKE_COMMAND} -P clean-all.cmake
)

check_library_exists(z    deflate     "" HAVE_ZLIB)
find_library(png REQUIRED)

add_library(log4cxx STATIC IMPORTED)
set_property(TARGET log4cxx PROPERTY IMPORTED_LOCATION "/usr/local/lib/liblog4cxx.a")

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

find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

include_directories(${ImageMagick_INCLUDE_DIRS})

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16 
                -DMAGICKCORE_HDRI_ENABLE=0)

include_directories("/usr/local/include")

link_directories("/usr/local/lib"
                 "/usr/X11/lib")

add_subdirectory("vendor/cNBT")

set(CMAKE_CXX_FLAGS "-Os -Wall -pedantic -Wno-unneeded-internal-declaration -Wno-c++11-extensions -ffast-math")
set(CMAKE_C_FLAGS "-Os -Wall -pedantic -std=c99")
add_subdirectory("vendor/json-spirit")

include_directories("vendor/cNBT/")
link_directories("${CMAKE_SOURCE_DIR}/vendor/cNBT")

include_directories("${CMAKE_SOURCE_DIR}/vendor/json-spirit/json_spirit/")

set(CMAKE_CXX_FLAGS "-Os -Wall -pedantic -std=c++98 -Wno-unneeded-internal-declaration -Wno-c++11-extensions")
set(CMAKE_C_FLAGS "-Os -Wall -pedantic -std=c99")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -pedantic -std=c++98 -Wno-unneeded-internal-declaration -Wno-c++11-extensions -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall -pedantic -std=c99")

aux_source_directory(src/ SOURCES)

configure_file(
  "${PROJECT_SOURCE_DIR}/src/config.hpp.in"
  "${PROJECT_SOURCE_DIR}/src/config.hpp"
)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(mcmap ${SOURCES})

target_link_libraries(mcmap 
  apr-1
  aprutil-1
  z 
  png 
  json_spirit
  nbt
  log4cxx
  ${ImageMagick_LIBRARIES}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_CONFIG_LIBRARY}
  ${Boost_FORMAT_LIBRARY}
  ${Boost_ITERATOR_LIBRARY}
  ${Boost_IOSTREAMS_LIBRARY}
  ${Boost_THREAD_LIBRARY}
)

if (${CMAKE_BUILD_TYPE} MATCHES DEBUG)
set(CMAKE_INSTALL_PREFIX ".")
endif()

add_dependencies(mcmap nbt json_spirit)

install(TARGETS mcmap RUNTIME DESTINATION bin)
install(DIRECTORY misc DESTINATION share/mcmap)
install(DIRECTORY doc DESTINATION share/mcmap)
