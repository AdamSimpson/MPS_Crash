cmake_minimum_required(VERSION 3.1)
project(mps_crash)

find_package(MPI REQUIRED)
find_package(CUDA REQUIRED)

file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/MPS_crash.c)

include_directories(${MPI_INCLUDE_PATH})

set_source_files_properties( ${SRC_FILES} PROPERTIES LANGUAGE CXX )
add_executable(MPS_crash ${SRC_FILES})

include_directories(${CUDA_INCLUDE_DIRS})
target_link_libraries(MPS_crash m)
target_link_libraries(MPS_crash ${CUDA_LIBRARIES})

target_link_libraries(MPS_crash ${MPI_C_LIBRARIES})
