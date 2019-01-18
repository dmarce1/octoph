project(octoph CXX)

cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)
find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
find_package(Boost REQUIRED)

set(dependencies Boost::boost)


set(source_files
    src/main.cpp
    src/basis/basis.cpp
    src/basis/legendre.cpp
    src/cell/cell_client.cpp
    src/cell/cell_directory.cpp
    src/cell/cell_location.cpp
    src/cell/cell_server.cpp
)

set(header_files
    ocotph/octoph.hpp
    ocotph/svec.hpp
    octoph/basis/basis.hpp
    octoph/basis/legendre.hpp
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




add_executable( legendre_test  src/basis/legendre.cpp octoph/basis/legendre.hpp  )
add_executable( basis_test  src/basis/basis.cpp octoph/basis/basis.hpp src/basis/legendre.cpp octoph/basis/legendre.hpp   )

target_include_directories(basis_test PRIVATE ${CMAKE_SOURCE_DIR}/octoph)
target_include_directories(legendre_test PRIVATE ${CMAKE_SOURCE_DIR}/octoph)

target_compile_options(basis_test PRIVATE ${FLAGS} -DTEST_BASIS)
target_compile_options(legendre_test PRIVATE ${FLAGS} -DTEST_LEGENDRE)

set( FLAGS -Wno-ignored-attributes -Wno-attributes -Wno-deprecated-declarations -Wno-unused-result -march=native)

target_include_directories(octoph        PRIVATE ${CMAKE_SOURCE_DIR}/octoph)

target_compile_options(octoph        PRIVATE ${FLAGS})


