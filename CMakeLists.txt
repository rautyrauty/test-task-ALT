cmake_minimum_required(VERSION 3.14)
project(TestTaskALT LANGUAGES CXX)
message(STATUS "the build of the ${PROJECT_NAME} has begun")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED OPENSSL_ROOT_DIR)
    if(WIN32)
        set(OPENSSL_ROOT_DIR "D:/Qt/Tools/OpenSSLv3/Win_x64")
    elseif(LINUX)
        set(OPENSSL_ROOT_DIR "/usr/lib/ssl")
    endif()
endif()

if(NOT DEFINED BOOST_ROOT)
    if(WIN32)
        set(BOOST_ROOT "D:/boost_1_82_0/boost_mingw_810_x64")
    elseif(LINUX)
        set(BOOST_ROOT "/usr/lib/ssl")
    endif()
endif()

add_library(MyLib SHARED)
target_sources(MyLib PRIVATE lib.cpp lib.hpp json.hpp)

message(STATUS "Boost include dir: ${Boost_INCLUDE_DIR}")
FIND_PACKAGE( Boost REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

message(STATUS "OpenSSL include dir ${OPENSSL_INCLUDE_DIR}")
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

if(WIN32)
    target_link_libraries(MyLib ws2_32)
endif()
target_link_libraries(MyLib ${Boost_LIBRARIES})
target_link_libraries(MyLib ${CMAKE_DL_LIBS} OpenSSL::SSL OpenSSL::Crypto)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC MyLib)
