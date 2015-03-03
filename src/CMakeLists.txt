find_package(OpenMP)

set(CMAKE_CXX_FLAGS "-O3 -std=c++11 -DRELEASE -funroll-loops -ffast-math")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -Wno-deprecated")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -Wno-deprecated")

file(GLOB_RECURSE INCLUDES
    "${POISSON_RECON_SOURCE_DIR}/src/*.h"
    "${POISSON_RECON_SOURCE_DIR}/src/*.cpp"
    "${POISSON_RECON_SOURCE_DIR}/src/*.inl")
list(REMOVE_ITEM INCLUDES ${POISSON_RECON_SOURCE_DIR}/src/PoissonRecon.cpp)
list(REMOVE_ITEM INCLUDES ${POISSON_RECON_SOURCE_DIR}/src/SurfaceTrimmer.cpp)

add_library(poisson_recon_lib SHARED ${INCLUDES})
set(LIBRARY_OUTPUT_PATH ${POISSON_RECON_SOURCE_DIR}/build/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${POISSON_RECON_SOURCE_DIR}/bin)

add_executable(PoissonRecon ${POISSON_RECON_SOURCE_DIR}/src/PoissonRecon.cpp)
target_link_libraries(PoissonRecon poisson_recon_lib)

add_executable(SurfaceTrimmer ${POISSON_RECON_SOURCE_DIR}/src/SurfaceTrimmer.cpp)
target_link_libraries(SurfaceTrimmer poisson_recon_lib)
