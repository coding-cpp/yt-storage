cmake_minimum_required(VERSION 3.16)
project(yt-storage VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_COMPILER "g++")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(USE_INSTALL_RESOURCE_PATH "Set resource path to install location" OFF)

find_package(OpenSSL REQUIRED)

add_executable(encrypt
  ./src/print.cpp
  ./src/parser.cpp
  ./src/encrypter.cpp

  ./example/encrypt.cpp
)
target_include_directories(encrypt
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(encrypt
  PRIVATE
    ${OPENSSL_LIBRARIES}
)