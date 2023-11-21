
cmake_minimum_required(VERSION 3.20)
project(lab_3_places)

set(CMAKE_CXX_STANDARD 20)



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


find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(lab_3_places ${Boost_LIBRARIES})


find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(lab_3_places ${OPENSSL_LIBRARIES})



