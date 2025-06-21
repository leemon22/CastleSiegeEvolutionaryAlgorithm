cmake_minimum_required(VERSION 3.31)
PROJECT(cec17)
ADD_EXECUTABLE(test "test.cc")
ADD_EXECUTABLE(testrandom "testrandom.cc")
ADD_EXECUTABLE(testsolis "testsolis.cc")
ADD_LIBRARY("cec17_test_func" SHARED "cec17_test_func.c" "cec17.c")
TARGET_LINK_LIBRARIES(test "cec17_test_func")
TARGET_LINK_LIBRARIES(testrandom "cec17_test_func")
TARGET_LINK_LIBRARIES(testsolis "cec17_test_func")

file(GLOB C_SOURCES
  "src/*.cpp"
)
INCLUDE_DIRECTORIES("inc")
ADD_EXECUTABLE(main "main.cpp" ${C_SOURCES})
TARGET_LINK_LIBRARIES(main "cec17_test_func")
