cmake_minimum_required(VERSION 3.16)
project(expresso VERSION 0.1.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(USE_INSTALL_RESOURCE_PATH "Set resource path to install location" OFF)

set(BUILD_SHARED_LIBS OFF)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE LIB_SOURCES "lib/**/src/*.cpp")

find_package(OpenSSL REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(encrypt
  ./example/encrypt.cpp

  ${SOURCES}
  ${LIB_SOURCES}
)
target_include_directories(encrypt
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:lib/logger/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/logger/include>
)
target_link_libraries(encrypt
  PRIVATE
    ${OPENSSL_LIBRARIES}
    ${OpenCV_LIBRARIES}
)

add_executable(decrypt
  ./example/decrypt.cpp

  ${SOURCES}
  ${LIB_SOURCES}
)
target_include_directories(decrypt
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:lib/logger/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/logger/include>
)
target_link_libraries(decrypt
  PRIVATE
    ${OPENSSL_LIBRARIES}
    ${OpenCV_LIBRARIES}
)