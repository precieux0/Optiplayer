add_executable(OptiPlayerTests test_player.cpp)
target_link_libraries(OptiPlayerTests OptiPlayerCore)
target_include_directories(OptiPlayerTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
add_test(NAME OptiPlayerTests COMMAND OptiPlayerTests)
