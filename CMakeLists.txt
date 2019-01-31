project(octoph CXX)

cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)
find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
find_package(Boost REQUIRED)

set(dependencies Boost::boost)

set( matrix_headers
    octoph/math/matrix/add.hpp
    octoph/math/matrix/characteristic.hpp
    octoph/math/matrix/column.hpp
    octoph/math/matrix/comatrix.hpp
    octoph/math/matrix/determinant.hpp
    octoph/math/matrix/inverse.hpp
    octoph/math/matrix/mask.hpp
    octoph/math/matrix/matrix.hpp
    octoph/math/matrix/neg.hpp
    octoph/math/matrix/product.hpp
    octoph/math/matrix/QR.hpp
    octoph/math/matrix/transpose.hpp
 )

set(source_files
    src/main.cpp
    src/math/basis/basis.cpp
    src/math/basis/legendre.cpp
    src/cell/cell_client.cpp
    src/cell/cell_directory.cpp
    src/cell/cell_location.cpp
    src/cell/cell_server.cpp
    src/exception/exception.cpp
    src/math/math.cpp
    src/math/matrix/matrix.cpp
)

set(header_files
    ocotph/octoph.hpp
    ocotph/svec.hpp
    octoph/math/basis/basis.hpp
    octoph/math/basis/legendre.hpp
    octoph/cell/cell_client.hpp
    octoph/cell/cell_directory.hpp
    octoph/cell/cell_location.hpp
    octoph/cell/cell_server.hpp
    octoph/exception/exception.hpp
 	${matrix_headers}
    octoph/math/math/math.hpp
    octoph/math/polynomial.hpp
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



set( FLAGS -Wno-ignored-attributes -Wno-attributes -Wno-deprecated-declarations -Wno-unused-result -std=c++17 -march=native )


add_executable            (basis_test src/math/basis/basis.cpp octoph/math/basis/basis.hpp src/math/basis/legendre.cpp octoph/math/basis/legendre.hpp src/math/math.cpp octoph/math/math.hpp  )
target_include_directories(basis_test PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options    (basis_test PRIVATE ${FLAGS} -DTEST_BASIS)

add_executable(            legendre_test src/math/basis/legendre.cpp octoph/math/basis/legendre.hpp src/math/math.cpp octoph/math/math.hpp )
target_include_directories(legendre_test PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(    legendre_test PRIVATE ${FLAGS} -DTEST_LEGENDRE)

add_executable            (math_test src/math/math.cpp octoph/math/math.hpp  octoph/math/polynomial.hpp src/exception/exception.cpp octoph/exception/exception.hpp)
target_include_directories(math_test PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options    (math_test PRIVATE ${FLAGS} -DTEST_MATH)


add_executable(            matrix_test src/math/matrix/matrix.cpp src/math/math.cpp  ${matrix_headers}  )
target_include_directories(matrix_test PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(    matrix_test PRIVATE ${FLAGS} -DTEST_MATRIX )

add_executable(            exception_test src/exception/exception.cpp octoph/exception/exception.hpp  )
target_include_directories(exception_test PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(    exception_test PRIVATE ${FLAGS} -DTEST_EXCEPTION )

target_include_directories(octoph        PRIVATE ${CMAKE_SOURCE_DIR})

target_compile_options(octoph        PRIVATE ${FLAGS})


