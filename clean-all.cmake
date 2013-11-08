set(cmake_generated ${CMAKE_BINARY_DIR}/CMakeCache.txt
                    ${CMAKE_BINARY_DIR}/cmake_install.cmake  
                    ${CMAKE_BINARY_DIR}/Makefile
                    ${CMAKE_BINARY_DIR}/CMakeFiles
                    ${CMAKE_BINARY_DIR}/CPackConfig.cmake
                    ${CMAKE_BINARY_DIR}/CPackSourceConfig.cmake

                    # technically, these are from make install, but who cares
                    ${CMAKE_BINARY_DIR}/bin
                    ${CMAKE_BINARY_DIR}/share
)

foreach(file ${cmake_generated})

  if (EXISTS ${file})
     file(REMOVE_RECURSE ${file})
  endif()

endforeach(file)
