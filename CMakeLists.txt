cmake_minimum_required(VERSION 2.6)
project(grasp-shaper)


# By default, build programs with compiler optimizations and debugging symbols
if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# libbarrett
find_package(Barrett REQUIRED)
include_directories(${BARRETT_INCLUDE_DIRS})
link_directories(${BARRETT_LIBRARY_DIRS})
add_definitions(${BARRETT_DEFINITIONS})

# curses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# everything else
set (ALL_LIBS ${ALL_LIBS} ${BARRETT_LIBRARIES} ${CURSES_LIBRARIES} boost_system)

# local includes
include_directories(${PROJECT_SOURCE_DIR}/robot)
include_directories(${PROJECT_SOURCE_DIR}/util)

# main program
add_executable(main main.cpp)
target_link_libraries(main ${ALL_LIBS})
