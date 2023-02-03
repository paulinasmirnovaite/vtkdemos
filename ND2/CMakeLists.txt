project (ND2)
cmake_minimum_required (VERSION 2.8)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

add_executable(glyphs glyphs.cpp)
target_link_libraries (glyphs ${VTK_LIBRARIES})


