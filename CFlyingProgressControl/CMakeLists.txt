cmake_minimum_required(VERSION 3.10)
project(FlyingProgressControl)

set(CMAKE_CXX_STANDARD 11)

add_executable(FlyingProgressControl main.cpp FlyingProgress.cpp FlyingProgress.h ListNode.h Motion.cpp Motion.h List.h test/TestList.h test/TestList.cpp test/TestMotion.h test/TestMotion.cpp test/TestFlyingProgress.h test/TestFlyingProgress.cpp)