cmake_minimum_required(VERSION 3.2)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
    project(bulk VERSION 1.1.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(bulk VERSION 1.1.1)
endif ()


# Setup application
add_executable(${PROJECT_NAME} main.cpp
                               ver.cpp
                               BulkPool.cpp
                               CmdInterpreter.cpp
                               CmdProcessor.cpp
                               ConsoleWriter.cpp
                               FileWriter.cpp
                               ../inc/BulkPool.h
                               ../inc/CmdInterpreter.h
                               ../inc/CmdProcessor.h
                               ../inc/ConsoleWriter.h
                               ../inc/FileWriter.h
                               ../inc/IObservable.h
                               ../inc/IStreamWriter.h
                               ../inc/ver.h)

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT petr_lutik@mail.ru)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

include (CPack)
