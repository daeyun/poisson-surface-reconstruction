find_package(OpenMP)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS} -fPIC -Wno-deprecated -g -std=c++11 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS} -fPIC -Wno-deprecated -Ofast -std=c++11 -DRELEASE -funroll-loops -ffast-math")

file(GLOB_RECURSE INCLUDES
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.inl")
list(REMOVE_ITEM INCLUDES ${PROJECT_SOURCE_DIR}/src/PoissonRecon.cpp)
list(REMOVE_ITEM INCLUDES ${PROJECT_SOURCE_DIR}/src/SurfaceTrimmer.cpp)

add_library(poisson_recon STATIC ${INCLUDES})

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(POISSON_RECON_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG} PARENT_SCOPE)
else()
    set(POISSON_RECON_CXX_FLAGS ${CMAKE_CXX_FLAGS_RELEASE} PARENT_SCOPE)
endif()
