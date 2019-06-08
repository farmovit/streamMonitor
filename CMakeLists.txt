cmake_minimum_required(VERSION 3.8)
project(streamMonitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

#enable_testing()
#find_package(GTest REQUIRED)
find_package(Boost COMPONENTS signals REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPTIMIZATION_FLAGS "-O0 -DDEBUG")
    message("-- Configuring debug build")
else()
    set(OPTIMIZATION_FLAGS "-O2 -DNDEBUG")
    message("-- Configuring release build")
endif()

set(DESIRED_WARNINGS "-Wall -Wextra -Wconversion -Wunreachable-code -Wuninitialized -pedantic-errors -Wold-style-cast -Wno-error=unused-variable -Wshadow -Wfloat-equal -Weffc++")

include_directories("${PROJECT_SOURCE_DIR}/src")

file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} -lpthread ${Boost_LIBRARIES})

#target_link_libraries(baseTest GTest::GTest GTest::Main)
#add_test(limitedBufferTests baseTest)
