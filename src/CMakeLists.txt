file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS *.cpp)
add_library(worldgen_core ${CORE_SOURCES})
target_include_directories(worldgen_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})