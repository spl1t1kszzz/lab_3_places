cmake_minimum_required(VERSION 3.27)
project(lab_3_places)

cmake_policy(SET CMP0144 NEW)
add_definitions(-D_WIN32_WINNT=0x0601)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab_3_places main.cpp)


set(BOOST_ROOT "C:/Program Files/Boost/boost_1_83_0")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(lab_3_places ${Boost_LIBRARIES})
