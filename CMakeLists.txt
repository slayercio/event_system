cmake_minimum_required(VERSION 3.10)
project(event_system CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB lib_headers include/evs/**.hpp)
add_library(event_system_lib INTERFACE ${lib_headers})
target_include_directories(event_system_lib INTERFACE include)

file(GLOB test_files src/test/**.cpp)
add_executable(event_system_test ${test_files})
target_link_libraries(event_system_test PUBLIC event_system_lib)
