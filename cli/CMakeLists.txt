file(GLOB PARSER_SOURCES CONFIGURE_DEPENDS parser/*.cpp)
add_executable(myproject main.cpp ${PARSER_SOURCES})
target_link_libraries(myproject PRIVATE worldgen_core)