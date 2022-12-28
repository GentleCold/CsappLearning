cmake_minimum_required(VERSION 3.16)
project(CsappLearning C)

set(CMAKE_C_STANDARD 11)

set(BUILD_PATH ${CMAKE_SOURCE_DIR}/build) # 自定义路径变量
set(LAB_PATH ${CMAKE_SOURCE_DIR}/labs)

set(EXECUTABLE_OUTPUT_PATH ${BUILD_PATH}) # EXE
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_PATH}) # LIB

add_executable(TestQuiz quiz/test_quiz.c)
add_executable(Mountain quiz/storage_mountain/mountain.c quiz/storage_mountain/clock.c quiz/storage_mountain/fcyc2.c)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_definitions(-DDEBUG)
endif()