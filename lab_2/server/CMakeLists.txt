cmake_minimum_required(VERSION 3.0) 
project(libhv-http) 
 
set(TARGET_NAME "libhv-http") 
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED TRUE) 
set(LIBHV_INCLUDE ${PROJECT_BINARY_DIR}/contrib/libhv/include/hv) 
 
find_package(OpenSSL REQUIRED) 
 
add_subdirectory(${PROJECT_SOURCE_DIR}/contrib) 
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wempty-body -Wredundant-move -O2") 
 
 
file(GLOB_RECURSE SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/src/base64/*.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/base64/*.hpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/base64/*.h 
${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ) 
 
set(SOURCE_DIR_HEADER "") 
execute_process( 
    COMMAND find src -type d 
    OUTPUT_VARIABLE SOURCE_DIR_HEADER 
    OUTPUT_STRIP_TRAILING_WHITESPACE 
    ERROR_QUIET 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}) 
    string(REPLACE "\n" ";" SOURCE_DIR_HEADER ${SOURCE_DIR_HEADER}) 
     
include_directories(${SOURCE_DIR_HEADER}) 
include_directories(${LIBHV_INCLUDE}) 
     
add_executable(${TARGET_NAME} ${SOURCES}) 
target_link_libraries(${TARGET_NAME} ${OPENSSL_LIBRARIES}) 
target_link_libraries(${TARGET_NAME} hv_static) 
 
message(${SOURCES}) 