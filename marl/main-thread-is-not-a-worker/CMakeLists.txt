cmake_minimum_required(VERSION 3.14)
project(MainThreadIsNotAWorker)
set(CMAKE_CXX_STANDARD 17)
message("Current source directory: ${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(${PROJECT_NAME} main.cc)
# marl
if (WIN32)
else()
set(path_to_marl "/Volumes/ExFAT/source/repos/marl")
target_include_directories(${PROJECT_NAME} PRIVATE ${path_to_marl}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${path_to_marl}/build)
target_link_libraries(${PROJECT_NAME} marl)  # linker to look for a library libmarl.a
endif()
#
