file(GLOB PARSER_SOURCES CONFIGURE_DEPENDS parser/*.cpp)
add_executable(cli main.cpp ${PARSER_SOURCES})
target_link_libraries(cli PRIVATE worldgen_core)