## libbarrett
find_package(Barrett REQUIRED)
include_directories(${BARRETT_INCLUDE_DIRS})
add_definitions(${BARRETT_DEFINITIONS})

include_directories(${PROJECT_SOURCE_DIR}/util)

add_library(grasper Grasper.cpp)
