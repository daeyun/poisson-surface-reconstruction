cmake_minimum_required(VERSION 2.8.4)

project(POISSON_RECON)
set(MEX_NAME "poissonRecon")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(MATLAB REQUIRED)
find_package(OpenMP REQUIRED)

if(MATLAB_FOUND)
  MESSAGE(STATUS "Matlab found at ${MATLAB_ROOT}")
else(MATLAB_FOUND)
  MESSAGE(FATAL_ERROR "Matlab not found")
endif(MATLAB_FOUND)

set(CMAKE_VERBOSE_MAKEFILE on)


set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
add_subdirectory(src)

set(MEX_OUTPUT_DIR "${PROJECT_SOURCE_DIR}")
set(MEX_OUTPUT_FILENAME "${MEX_NAME}${MATLAB_MEX_EXTENSION}")


add_custom_target(
    "${PROJECT_NAME}_MEX" ALL
    DEPENDS ${MEX_OUTPUT_DIR}/${MEX_OUTPUT_FILENAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(MEX_BUILD_FLAGS -g -v)
else()
    set(MEX_BUILD_FLAGS -O -v -DNDEBUG -DRELEASE)
endif()

add_custom_command(
    OUTPUT ${MEX_OUTPUT_DIR}/${MEX_OUTPUT_FILENAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS poisson_recon
    COMMAND ${MATLAB_MEX_EXECUTABLE}
    poissonRecon.mex.cc -lpoisson_recon
    LDFLAGS=\"\\$$LDFLAGS ${OpenMP_CXX_FLAGS} -std=c++0x\"
    CXXFLAGS=\"\\$$CXXFLAGS ${OpenMP_CXX_FLAGS} -std=c++0x\"
    -largeArrayDims -cxx
    -outdir ${MEX_OUTPUT_DIR} -output ${MEX_OUTPUT_FILENAME}
    -L${LIBRARY_OUTPUT_PATH} ${MEX_BUILD_FLAGS}
)
