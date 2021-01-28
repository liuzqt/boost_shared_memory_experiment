cmake_minimum_required(VERSION 3.0.0)
project(new_project VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_package(Boost 1.58.0 REQUIRED COMPONENTS serialization filesystem thread)


file(GLOB TARGET_SRC "./src/*.cpp" )

include_directories(include)


find_package(Threads REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_executable(main_map "./src/main_map.cpp")

target_link_libraries(main_map PRIVATE ${CMAKE_THREAD_LIBS_INIT} rt)

add_executable(main_vec "./src/main_vec.cpp")

target_link_libraries(main_vec PRIVATE ${CMAKE_THREAD_LIBS_INIT} rt)
