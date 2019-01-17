project(octoph CXX)

cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)
find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
find_package(Boost REQUIRED)

set(dependencies Boost::boost)
# Octoph executable
set(source_files
    src/main.cpp
    src/basis/basis.cpp
    src/cell/cell_client.cpp
    src/cell/cell_directory.cpp
    src/cell/cell_location.cpp
    src/cell/cell_server.cpp
)

set(header_files
    ocotph/octoph.hpp
    octoph/basis/basis.hpp
    octoph/cell/cell_client.hpp
    octoph/cell/cell_directory.hpp
    octoph/cell/cell_location.hpp
    octoph/cell/cell_server.hpp
)

add_hpx_executable(
  octoph
  DEPENDENCIES
    ${dependencies}
  SOURCES
    ${source_files}
  HEADERS
    ${header_files}
  FOLDER  "Octoph"
)

target_include_directories(octoph PRIVATE ${CMAKE_SOURCE_DIR}/octoph)

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(octoph PRIVATE
      -Wno-ignored-attributes -Wno-attributes -Wno-deprecated-declarations
      -Wno-unused-result -ffast-math)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
      #set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -ipo -Wunused-result")
      #set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Ofast -ipo")
      target_compile_options(octoph -Wno-attributes -Wno-deprecated)
  endif()


