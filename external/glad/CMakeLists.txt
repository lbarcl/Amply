# Add the source files
add_library(glad glad.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)