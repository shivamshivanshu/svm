cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME Svm)
set(PROJECT_LIB_NAME Svm_Lib)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_FLAGS -Wall -Wextra -Wpedantic -Werror)
set(DEBUG_FLAGS -O0 -g)
set(RELEASE_FLAGS -O2 -DNDEBUG)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type")
endif()

option(BUILD_TESTING "Build tests" ON)
file(GLOB_RECURSE LIB_SOURCES src/*.cpp)

add_library(${PROJECT_LIB_NAME} STATIC ${LIB_SOURCES})
target_include_directories(${PROJECT_LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_compile_options(${PROJECT_LIB_NAME} PRIVATE
    ${COMMON_FLAGS} 
    $<$<CONFIG:Debug>:${DEBUG_FLAGS}> 
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>
)
target_compile_features(${PROJECT_LIB_NAME} PRIVATE cxx_std_20)

add_executable(${PROJECT_NAME} entry.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIB_NAME})
target_compile_options(${PROJECT_NAME} PRIVATE
    ${COMMON_FLAGS} 
    $<$<CONFIG:Debug>:${DEBUG_FLAGS}> 
    $<$<CONFIG:Release>:${RELEASE_FLAGS}>
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    file(GLOB_RECURSE TEST_SOURCES test/*.cpp)

    if(TEST_SOURCES)
        add_executable(UnitTests ${TEST_SOURCES})
        target_link_libraries(UnitTests PRIVATE ${PROJECT_LIB_NAME} gtest_main)
        target_compile_options(UnitTests PRIVATE
            ${COMMON_FLAGS} 
            $<$<CONFIG:Debug>:${DEBUG_FLAGS}> 
            $<$<CONFIG:Release>:${RELEASE_FLAGS}>
        )
        set_target_properties(UnitTests PROPERTIES POSITION_INDEPENDENT_CODE ON)
        target_compile_features(UnitTests PRIVATE cxx_std_20)
        add_test(NAME AllTests COMMAND UnitTests)
    endif()
endif()

