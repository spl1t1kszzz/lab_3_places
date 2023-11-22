
cmake_minimum_required(VERSION 3.20)
project(lab_3_places)

cmake_policy(SET CMP0144 NEW)
add_definitions(-D_WIN32_WINNT=0x0601)
set(CMAKE_CXX_STANDARD 23)



add_executable(lab_3_places
        main.cpp
        JSON/json.hpp
        Finder.cpp
        Finder.hpp
        API/Weather.cpp
        API/Weather.hpp
        API/HTTP_Request.cpp
        API/HTTP_Request.hpp
        Config.hpp
        JSON/JSON_Formatter.cpp
        JSON/JSON_Formatter.hpp
        API/Interesting_place.cpp
        API/Interesting_place.hpp
)


set(BOOST_ROOT "C:/Program Files/Boost/boost_1_83_0")
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(lab_3_places ${Boost_LIBRARIES})


find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(lab_3_places ${OPENSSL_LIBRARIES})


