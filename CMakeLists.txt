cmake_minimum_required(VERSION 3.8)
project(octree_cpp CXX)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_TESTS "Build all tests" ON)
option(BUILD_TESTS_WITH_COVERAGE "Build all tests" ON)

add_library(octree_cpp INTERFACE)
target_include_directories(octree_cpp INTERFACE include/)

if (BUILD_TESTS)
    include(ExternalProject)

    ExternalProject_Add(
            glm
            PREFIX ${CMAKE_BINARY_DIR}/glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
            LOG_DOWNLOAD ON
    )
    ExternalProject_Get_Property(glm source_dir)
    set(GlmIncludeDir ${source_dir})


    if (BUILD_TESTS_WITH_COVERAGE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --pedantic") # enabling coverage
    endif ()
    find_package(Threads)
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(octree_tests include/octree_cpp.h tests/octree_tests.cpp)
    target_link_libraries(octree_tests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} octree_cpp)
    target_include_directories(octree_tests PRIVATE ${GlmIncludeDir})

    add_dependencies(octree_tests glm)
    message(${GlmIncludeDir})


endif ()