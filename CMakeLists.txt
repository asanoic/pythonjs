cmake_minimum_required(VERSION 3.13)
project("python.js")

set(CMAKE_VERBOSE_MAKEFILE ON)

set(pwd ${CMAKE_CURRENT_LIST_DIR})

list(APPEND src
    "${pwd}/src/python.js.h"
    "${pwd}/src/main.cpp"
    "${pwd}/src/utils.h"
    "${pwd}/src/utils.cpp"
    "${pwd}/src/configure.h"
    "${pwd}/src/configure.cpp"
    "${pwd}/src/napi-run.cpp"
    "${pwd}/src/napi-import.cpp"
    "${pwd}/src/napi-call.cpp"
)

add_library(${CMAKE_PROJECT_NAME} MODULE ${src})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".node" PREFIX "")
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)

add_library(node-lib INTERFACE)
target_compile_definitions(node-lib INTERFACE NODE_ADDON_API_DISABLE_DEPRECATED)
target_include_directories(node-lib INTERFACE "${pwd}/napi/include")

add_library(python-lib INTERFACE)

if(WIN32)
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "-fno-keep-inline-dllexport" "-fexceptions" "-mthreads")
    target_link_options(${CMAKE_PROJECT_NAME} PUBLIC "-Wl,--exclude-all-symbols")

    target_link_libraries(node-lib INTERFACE "-L${pwd}/napi/lib" "-lnode")

    target_include_directories(python-lib INTERFACE "${pwd}/python-capi/include")
    target_link_libraries(python-lib INTERFACE "-L${pwd}/python-capi/win" "-lpython38")

else()
    find_package(Python3 REQUIRED COMPONENTS Development)
    target_link_libraries(python-lib INTERFACE Python3::Python)

endif()

target_link_libraries(${CMAKE_PROJECT_NAME} node-lib python-lib)

add_custom_target(copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${pwd}/src/test.js"
    "${pwd}/src/pycode.py"
    "${pwd}/src/python.js.d.ts"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

## other files
add_custom_target(other-files SOURCES
    "${pwd}/src/test.js"
    "${pwd}/src/pycode.py"
    "${pwd}/src/python.js.d.ts"
)




