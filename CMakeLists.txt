cmake_minimum_required(VERSION 3.15)
project(VerifyParallelLibs CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(TBB CONFIG REQUIRED)

add_executable(verify_parallel main.cpp)

target_link_libraries(verify_parallel PRIVATE TBB::tbb)
target_compile_definitions(verify_parallel PRIVATE TBB_FOUND)
