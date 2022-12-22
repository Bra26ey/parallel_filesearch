project(file_search)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic")

find_package(Boost 1.65.1 REQUIRED COMPONENTS system thread filesystem)
find_package(Threads)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS}
                    include)

add_executable(file_search main.cpp)

target_link_libraries(file_search
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${Boost_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK --coverage -lboost_thread -pthread")