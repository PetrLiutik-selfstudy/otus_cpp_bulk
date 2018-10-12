cmake_minimum_required(VERSION 3.2)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(${PROJECT_NAME} test_main.cpp
                               ../src/BulkPool.cpp
                               ../src/CmdInterpreter.cpp
                               ../src/CmdProcessor.cpp
                               ../src/ConsoleWriter.cpp
                               ../src/FileWriter.cpp
                               ../src/ver.cpp
                               ../inc/BulkPool.h
                               ../inc/CmdInterpreter.h
                               ../inc/CmdProcessor.h
                               ../inc/ConsoleWriter.h
                               ../inc/FileWriter.h
                               ../inc/IObservable.h
                               ../inc/IStreamWriter.h
                               ../inc/ver.h)

set_target_properties(${PROJECT_NAME}  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} gtest pthread )

add_test(ver_test_case ${PROJECT_NAME})
add_test(cmd_processor_test_case ${PROJECT_NAME})
add_test(console_writer_test_case ${PROJECT_NAME})
