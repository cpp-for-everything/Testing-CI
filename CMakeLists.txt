cmake_minimum_required(VERSION 3.15)
project(VerifyParallelLibs CXX)

set(CMAKE_CXX_STANDARD 20)

if(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
  set(OpenMP_CXX_LIB_NAMES "omp")
  set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib") # or /usr/local/opt/libomp/lib/libomp.dylib on Intel
  set(OpenMP_INCLUDE_DIRS "/opt/homebrew/opt/libomp/include")         # or /usr/local/opt/libomp/include
endif()
find_package(OpenMP)

find_package(TBB)

add_executable(verify_parallel main.cpp)

if(OpenMP_CXX_FOUND)
  target_include_directories(verify_parallel PRIVATE ${OpenMP_INCLUDE_DIRS})
  target_compile_options(verify_parallel PRIVATE ${OpenMP_CXX_FLAGS})
  target_link_libraries(verify_parallel PRIVATE ${OpenMP_omp_LIBRARY})
else()
  message(WARNING "OpenMP not available on this compiler.")
endif()

if(TBB_FOUND)
  target_link_libraries(verify_parallel PRIVATE TBB::tbb)
  target_compile_definitions(verify_parallel PRIVATE TBB_FOUND)
endif()
