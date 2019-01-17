project(octoph CXX)

cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)
find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
find_package(Boost REQUIRED)

set(dependencies Boost::boost)


set(source_files
    src/main.cpp
    src/cell/cell_client.cpp
    src/cell/cell_directory.cpp
    src/cell/cell_location.cpp
    src/cell/cell_server.cpp
)

set(header_files
    ocotph/octoph.hpp
    octoph/basis/basis.hpp
    octoph/basis/legendre.hpp
    octoph/cell/cell_client.hpp
    octoph/cell/cell_directory.hpp
    octoph/cell/cell_location.hpp
    octoph/cell/cell_server.hpp
)

set(dependencies basis)

add_hpx_executable(
  octoph1d
  DEPENDENCIES
    ${dependencies}
  SOURCES
    ${source_files}
  HEADERS
    ${header_files}
  FOLDER  "Octoph1d"
)


add_hpx_executable(
  octoph2d
  DEPENDENCIES
    ${dependencies}
  SOURCES
    ${source_files}
  HEADERS
    ${header_files}
  FOLDER  "Octoph2d"
)


add_hpx_executable(
  octoph3d
  DEPENDENCIES
    ${dependencies}
  SOURCES
    ${source_files}
  HEADERS
    ${header_files}
  FOLDER  "Octoph3d"
)


add_executable( legendre_test 
	src/basis/legendre.cpp octoph/basis/legendre.hpp 
)

add_library( basis 
	src/basis/basis.cpp 
	src/basis/legendre.cpp 
	octoph/basis/basis.hpp 
	octoph/basis/legendre.hpp 
)

target_include_directories(basis PRIVATE ${CMAKE_SOURCE_DIR}/octoph)
target_include_directories(legendre_test PRIVATE ${CMAKE_SOURCE_DIR}/octoph)
target_include_directories(octoph1d      PRIVATE ${CMAKE_SOURCE_DIR}/octoph)
target_include_directories(octoph2d      PRIVATE ${CMAKE_SOURCE_DIR}/octoph)
target_include_directories(octoph3d      PRIVATE ${CMAKE_SOURCE_DIR}/octoph)

target_compile_options(legendre_test PRIVATE  -Wno-ignored-attributes -Wno-attributes -Wno-deprecated-declarations -Wno-unused-result -DTESTME)
target_compile_options(octoph1d      PRIVATE -DOCTOPH_1D -Wno-ignored-attributes -Wno-attributes -Wno-deprecated-declarations -Wno-unused-result)
target_compile_options(octoph2d      PRIVATE -DOCTOPH_2D -Wno-ignored-attributes -Wno-attributes -Wno-deprecated-declarations -Wno-unused-result)
target_compile_options(octoph3d      PRIVATE -DOCTOPH_3D -Wno-ignored-attributes -Wno-attributes -Wno-deprecated-declarations -Wno-unused-result)


